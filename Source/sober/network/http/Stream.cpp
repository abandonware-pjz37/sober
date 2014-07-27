// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/Stream.hpp>

#include <leathers/push>
#include <leathers/all>
# include <boost/log/sources/record_ostream.hpp> // BOOST_LOG
# include <network/uri.hpp>
#include <leathers/pop>

#include <sober/log/Severity.hpp>
#include <sober/network/Engine.ipp>
#include <sober/network/http/ConnectManager.ipp> // connect_manager_
#include <sober/network/http/Request.ipp> // request_
#include <sober/network/http/Response.ipp> // response_
#include <sober/network/http/delegate/Interface.hpp>
#include <sober/asserts/sober_assert.hpp> // SOBER_ASSERT_TRUE

namespace sober {
namespace network {
namespace http {

Stream::Stream(Engine& engine):
    log_("sober.network.http.Stream", this),
    delegate_(nullptr),
    socket_(engine.io_service),
    in_progress_(false),
    force_stop_(false),
    connect_manager_(engine),
    resolve_handler_(*this),
    connect_handler_(*this),
    write_handler_(*this),
    read_some_handler_(*this),
    restart_handler_(*this),
    watchdog_handler_(*this),
    restart_timer_(engine.io_service),
    watchdog_timer_(engine.io_service) {
}

void Stream::set_endpoint(const ::network::uri& uri) {
  connect_manager_.set_endpoint(uri);

  if (uri.path()) {
    std::string path = uri.path()->to_string();
    if (path.empty()) {
      path = "/";
    }
    request.set_path(path.c_str());
  }
  else {
    request.set_path("/");
  }

  if (uri.query()) {
    request.set_query(uri.query()->to_string());
  }
  else {
    request.clear_query();
  }
}

void Stream::set_endpoint(const std::string& endpoint) {
  set_endpoint(::network::uri(endpoint));
}

void Stream::set_delegate(delegate::Interface& delegate) {
  delegate_ = std::addressof(delegate);
  response.set_delegate(delegate);
}

void Stream::async_start(ExtraSuccessHandler handler) {
  BOOST_LOG(log_.debug) << "async start";
  extra_success_handler_ = std::move(handler);

  if (delegate_ != nullptr) {
    delegate_->on_start();
  }

  // only one request is allowed, check there is no request in process already
  check_not_in_progress();

  in_progress_ = true;
  check_in_progress(); // sanity check

  force_stop_ = false;

  if (delegate_ != nullptr) {
    watchdog_timer_.expires_from_now(delegate_->watchdog_period());
    watchdog_timer_.async_wait(watchdog_handler_);
  }

  start();
}

void Stream::cancel() {
  BOOST_LOG(log_.info) << "cancel";

  force_stop_ = true;
  in_progress_ = false;
  check_not_in_progress(); // sanity check

  socket_.close();
  watchdog_timer_.cancel();
  restart_timer_.cancel();
}

const Statistic& Stream::statistic() const BOOST_NOEXCEPT {
  return statistic_;
}

void Stream::clear_statistic() BOOST_NOEXCEPT {
  statistic_.clear();
}

void Stream::check_not_in_progress() {
  if (in_progress_) {
    throw std::runtime_error("Request in progress, call 'Engine::run'");
  }
}

void Stream::check_in_progress() {
  if (!in_progress_) {
    throw std::runtime_error("Operation in progress but flag is not set");
  }
}

bool Stream::stop_condition(const Error& error, const char* operation) {
  if (!force_stop_ && !error) {
    return false;
  }

  if (force_stop_) {
    BOOST_LOG(log_.info) << "force stop (operation: " << operation << ")";
  }
  else {
    assert(error);
    BOOST_LOG(log_.error) << "operation `" << operation << "` failed: " <<
        error.message();

    if ((delegate_ != nullptr) && delegate_->restart_on_error(error)) {
      restart_operation();
      return true;
    }
  }

  cancel();
  return true;
}

void Stream::restart_operation() {
  check_in_progress();
  assert(!force_stop_);
  assert(delegate_ != nullptr);

  BOOST_LOG(log_.info) << "restart operation";
  statistic_.on_restart();

  // Note: Do not start operation synchronously.
  // connect_manager_.clear_connected must be called before next operation
  // started (otherwise cached value will be used without new connection
  // initialized).

  // Scenario:
  //   * operation failed with EOF (Stream::read_some_handler) ->
  //     * stop_condition = true (must reset connect_manager_) ->
  //       * Stream::start -> resolve_handler -> connect_handler (!no reconnect)

  // Expected:
  //   * operation failed with EOF (Stream::read_some_handler) ->
  //     * stop_condition = true, schedule async start ->
  //       * connect_handler_.clear_connected() ->
  //         * (wait async operation...)
  //           * Stream::start -> resolve_handler -> connect_handler, reconnect
  restart_timer_.expires_from_now(delegate_->restart_pause());
  restart_timer_.async_wait(restart_handler_);
}

void Stream::start() {
  BOOST_LOG(log_.debug) << "start";

  response.clear();

  const Error error;
  if (stop_condition(error, "start")) {
    return;
  }

  check_in_progress();
  connect_manager_.resolve(resolve_handler_);
}

void Stream::resolve_handler(const Error &error, Resolver::iterator iterator) {
  if (stop_condition(error, "resolve")) {
    return;
  }

  if (!connect_manager_.is_connected()) {
    BOOST_LOG(log_.info) << "resolved:";
    int i = 1;
    for (Resolver::iterator it = iterator; it != Resolver::iterator(); it++) {
      BOOST_LOG(log_.info) << "  " << i << ": " << it->endpoint();
      ++i;
    }

    if (delegate_ != nullptr) {
      iterator = delegate_->pick_connect_endpoint(iterator);
    }
  }

  check_in_progress();
  connect_manager_.connect(connect_handler_, socket_, iterator);
}

void Stream::connect_handler(const Error &error, Resolver::iterator iterator) {
  if (stop_condition(error, "connect")) {
    if (error != boost::asio::error::operation_aborted) {
      // do not clear resolved cache if operation cancelled by user,
      // just try to get another iterator
      BOOST_LOG(log_.info) << "clear resolved";
      connect_manager_.clear_resolved();
    }
    return;
  }

  connect_manager_.on_successful_connect(iterator);
  check_in_progress();

  BOOST_LOG(log_.info) << "write request";
  request.async_write(socket_, connect_manager_.host(), write_handler_);
}

void Stream::write_handler(const Error &error, std::size_t bytes_transferred) {
  if (stop_condition(error, "write")) {
    connect_manager_.clear_connected();
    return;
  }

  BOOST_LOG(log_.info) << "write done (" << bytes_transferred << " bytes)";

  check_in_progress();

  // number of writen bytes must be equals to request size
  request.verify_size_on_write_done(bytes_transferred);

  response.async_read_some(socket_, read_some_handler_);
}

void Stream::read_some_handler(
    const Error &error, std::size_t bytes_transferred
) {
  if (!error) {
    BOOST_LOG(log_.debug) << bytes_transferred << " bytes read";
  }

  if (stop_condition(error, "read")) {
    connect_manager_.clear_connected();
    return;
  }

  check_in_progress();

  const bool do_stop = response.on_read(bytes_transferred);
  if (!do_stop) {
    response.async_read_some(socket_, read_some_handler_);
    return;
  }

  using StatusCode = response::attribute::StatusCode;
  const StatusCode status_code = response.header().status_line.status_code;

  // Redirection
  if (status_code == StatusCode::FOUND) {
    if (delegate_ == nullptr) {
      BOOST_LOG(log_.info) << "Redirection not allowed (no delegate)";
    }
    else if(!delegate_->allow_redirection()) {
      BOOST_LOG(log_.info) << "Redirection not allowed by delegate";
    }
    else if (!response.header().location.is_initialized()) {
      BOOST_LOG(log_.info) << "Redirect response without Location header";
    }
    else {
      const ::network::uri& location = response.header().location.get();
      BOOST_LOG(log_.info) << "Redirect to " << location;
      set_endpoint(location);
      return start();
    }
  }

  const bool success =
      (status_code == StatusCode::OK) ||
      (status_code == StatusCode::CREATED)
  ;

  if (!success) {
    BOOST_LOG(log_.info) << "Status code is not OK: " << status_code;
    if ((delegate_ != nullptr) && delegate_->restart_on_error(status_code)) {
      BOOST_LOG(log_.info) << "restart operation (status code)";
      return restart_operation();
    }
  }

  in_progress_ = false;
  check_not_in_progress(); // sanity check

  watchdog_timer_.cancel();

  BOOST_LOG(log_.info) << "read done (" << bytes_transferred << " bytes)";

  if (delegate_ != nullptr && success) {
    delegate_->on_success();
    if (extra_success_handler_) {
      extra_success_handler_();
    }
  }
}

void Stream::watchdog_handler() {
  assert(in_progress_);
  assert(!force_stop_);

  if (delegate_ == nullptr) {
    return;
  }

  if (!delegate_->force_stop()) {
    const bool can_start_reconnect =
        connect_manager_.is_resolved() &&
        !connect_manager_.is_connected();

    if (can_start_reconnect && delegate_->force_reconnect()) {
      BOOST_LOG(log_.info) << "force reconnect";
      statistic_.on_reconnect();
      socket_.close();
    }
    watchdog_timer_.expires_from_now(delegate_->watchdog_period());
    watchdog_timer_.async_wait(watchdog_handler_);
    return;
  }

  force_stop_ = true;

  const Error error;
  const bool verify_true = stop_condition(error, "watchdog handler");

  SOBER_ASSERT_TRUE_VAR(verify_true);
}

} // namespace http
} // namespace network
} // namespace sober
