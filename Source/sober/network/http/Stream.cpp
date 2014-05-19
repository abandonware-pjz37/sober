// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/Stream.hpp>

#include <boost/log/sources/record_ostream.hpp> // BOOST_LOG
#include <network/uri.hpp>
#include <sober/log/Severity.hpp>
#include <sober/network/Engine.ipp>
#include <sober/network/http/ConnectManager.ipp> // connect_manager_
#include <sober/network/http/Request.ipp> // request_
#include <sober/network/http/Response.ipp> // response_
#include <sober/network/http/delegate/Interface.hpp>

namespace sober {
namespace network {
namespace http {

Stream::Stream(Engine& engine, Request& request, Response& response):
    log_debug_(*this, log::Severity::DEBUG),
    log_info_(*this, log::Severity::INFO),
    log_error_(*this, log::Severity::ERROR),
    delegate_(nullptr),
    socket_(engine.io_service),
    in_progress_(false),
    force_stop_(false),
    request_(request),
    response_(response),
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
    request_.set_path(path.c_str());
  }
  else {
    request_.set_path("/");
  }

  if (uri.query()) {
    request_.set_query(uri.query()->to_string());
  }
  else {
    request_.clear_query();
  }
}

void Stream::set_endpoint(const std::string& endpoint) {
  set_endpoint(::network::uri(endpoint));
}

void Stream::set_delegate(delegate::Interface& delegate) {
  delegate_ = &delegate;
}

void Stream::async_start() {
  BOOST_LOG(log_debug_) << "async start";

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

const Statistic& Stream::statistic() const noexcept {
  return statistic_;
}

void Stream::clear_statistic() noexcept {
  statistic_.clear();
}

const char* Stream::log_name() const noexcept {
  return "sober.network.http.Stream";
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
    BOOST_LOG(log_info_) << "force stop (operation: " << operation << ")";
  }
  else {
    assert(error);
    BOOST_LOG(log_error_) << "operation `" << operation << "` failed: " <<
        error.message();

    if ((delegate_ != nullptr) && delegate_->restart_on_error()) {
      restart_operation();
      return true;
    }
  }

  force_stop_ = true;
  in_progress_ = false;
  check_not_in_progress(); // sanity check

  socket_.close();
  watchdog_timer_.cancel();
  restart_timer_.cancel();

  return true;
}

void Stream::restart_operation() {
  check_in_progress();
  assert(!force_stop_);
  assert(delegate_ != nullptr);

  BOOST_LOG(log_info_) << "restart operation";
  statistic_.on_restart();

  boost::posix_time::time_duration pause = delegate_->restart_pause();

  if (pause.ticks() == 0) {
    start();
  }
  else {
    restart_timer_.expires_from_now(pause);
    restart_timer_.async_wait(restart_handler_);
  }
}

void Stream::start() {
  BOOST_LOG(log_debug_) << "start";

  response_.clear();

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
    BOOST_LOG(log_info_) << "resolved: ";
    int i = 1;
    for (Resolver::iterator it = iterator; it != Resolver::iterator(); it++) {
      BOOST_LOG(log_info_) << "  " << i << ": " << it->endpoint();
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
      BOOST_LOG(log_info_) << "clear resolved";
      connect_manager_.clear_resolved();
    }
    return;
  }

  connect_manager_.on_successful_connect(iterator);
  check_in_progress();

  BOOST_LOG(log_info_) << "write request";
  request_.async_write(socket_, connect_manager_.host(), write_handler_);
}

void Stream::write_handler(const Error &error, std::size_t bytes_transferred) {
  if (stop_condition(error, "write")) {
    connect_manager_.clear_connected();
    return;
  }

  BOOST_LOG(log_info_) << "write done (" << bytes_transferred << " bytes)";

  check_in_progress();

  // number of writen bytes must be equals to request size
  request_.verify_size_on_write_done(bytes_transferred);

  response_.async_read_some(socket_, read_some_handler_);
}

void Stream::read_some_handler(
    const Error &error, std::size_t bytes_transferred
) {
  if (!error) {
    BOOST_LOG(log_debug_) << bytes_transferred << " bytes read";
  }

  if (stop_condition(error, "read")) {
    connect_manager_.clear_connected();
    return;
  }

  check_in_progress();

  const bool do_stop = response_.on_read(bytes_transferred);
  if (!do_stop) {
    response_.async_read_some(socket_, read_some_handler_);
    return;
  }

  using StatusCode = response::attribute::StatusCode;
  const StatusCode status_code = response_.header().status_line.status_code;
  const bool success = (status_code == StatusCode::OK);
  if (!success) {
    BOOST_LOG(log_info_) << "Status code is not OK: " << status_code;
    if ((delegate_ != nullptr) && delegate_->restart_on_error()) {
      BOOST_LOG(log_info_) << "restart operation (status code)";
      return restart_operation();
    }
  }

  in_progress_ = false;
  check_not_in_progress(); // sanity check

  watchdog_timer_.cancel();

  BOOST_LOG(log_info_) << "read done (" << bytes_transferred << " bytes)";

  if (delegate_ != nullptr && success) {
    delegate_->on_success();
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
      BOOST_LOG(log_info_) << "force reconnect";
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

  assert(verify_true);
}

} // namespace http
} // namespace network
} // namespace sober
