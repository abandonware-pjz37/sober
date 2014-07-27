#ifndef SOBER_NETWORK_HTTP_STREAM_HPP_
#define SOBER_NETWORK_HTTP_STREAM_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/Stream.fpp>

#include <leathers/push>
#include <leathers/all>
# include <boost/asio.hpp>
#include <leathers/pop>

#include <sober/log/Logger.hpp>
#include <sober/network/Engine.fpp>
#include <sober/network/http/ConnectManager.hpp>
#include <sober/network/http/ExtraSuccessHandler.hpp>
#include <sober/network/http/Request.hpp>
#include <sober/network/http/Response.hpp>
#include <sober/network/http/Statistic.hpp>
#include <sober/network/http/delegate/Interface.fpp>

namespace network {
class uri;
} // namespace network

namespace sober {
namespace network {
namespace http {

/**
  * @brief Class to control @c request::Request, @c response::Response,
  * @c ConnectManager and timers activity
  * @code
  * // 1. Init
  * Engine engine;
  * delegate::String sink = ...; // any class with delegate::Interface interface
  * Stream stream(engine);
  * stream.set_delegate(sink);
  * // 2. Configure request and response
  * stream.set_endpoint(...);
  * stream.request.set_method(...); // opt
  * stream.request.set_path(...); // opt
  * stream.request.set_query(...); // opt
  * stream.response.set_buffer_size(...); // opt
  * // 3. Init request asynchronously
  * stream.async_start(...);
  * // 4. Process
  * engine.run();
  * // 5. Response (or using delegate::Interface::on_success)
  * if (sink.ready()) {
  *   // Check stream.response.header().status_line.status_code if needed
  *   auto message = sink.body();
  * }
  * @endcode
  */
class Stream {
 public:
  using Error = boost::system::error_code;

  Stream(Engine&);

  Stream(Stream&&) = delete;
  Stream& operator=(Stream&&) = delete;

  Stream(const Stream&) = delete;
  Stream& operator=(const Stream&) = delete;

  //@{
  /**
    * @note Set both @c ConnectManager and @c Request
    * @brief
    * 1. Set host and port to @c ConnectManager
    * 2. Set host and query to @c Request
    */
  void set_endpoint(const std::string& uri);
  void set_endpoint(const ::network::uri& uri);
  //@}

  /**
    * @note Custom behaviour (timers, retry, success handle, ...)
    */
  void set_delegate(delegate::Interface& delegate);

  /**
    * @brief Add asynchorous operation to @c Engine
    */
  void async_start(ExtraSuccessHandler handler = nullptr);

  /**
    * @brief Cancel current operation
    */
  void cancel();

  //@{
  /**
    * @brief HTTP-stream statistic
    */
  const Statistic& statistic() const BOOST_NOEXCEPT;
  void clear_statistic() BOOST_NOEXCEPT;
  //@}

  Request request;
  Response response;

 private:
  using Resolver = boost::asio::ip::tcp::resolver;

  class ResolveHandler {
   public:
    ResolveHandler(Stream& stream): stream_(stream) {
    }

    void operator()(const Error& error, Resolver::iterator iterator) {
      stream_.resolve_handler(error, iterator);
    }

   private:
    Stream& stream_;
  };

  class ConnectHandler {
   public:
    ConnectHandler(Stream& stream): stream_(stream) {
    }

    void operator()(const Error& error, Resolver::iterator iterator) {
      stream_.connect_handler(error, iterator);
    }

   private:
    Stream& stream_;
  };

  class WriteHandler {
   public:
    WriteHandler(Stream& stream): stream_(stream) {
    }

    void operator()(const Error& error, std::size_t bytes_transferred) {
      stream_.write_handler(error, bytes_transferred);
    }

   private:
    Stream& stream_;
  };

  class ReadSomeHandler {
   public:
    ReadSomeHandler(Stream& stream): stream_(stream) {
    }

    void operator()(const Error& error, std::size_t bytes_transferred) {
      stream_.read_some_handler(error, bytes_transferred);
    }

   private:
    Stream& stream_;
  };

  class RestartHandler {
   public:
    RestartHandler(Stream& stream): stream_(stream) {
    }

    void operator()(const Error& error) {
      if (!error) {
        return stream_.start();
      }
      if (error == boost::asio::error::operation_aborted) {
        // ignore this error (cancel by timer)
        return;
      }
      throw std::runtime_error("RestartHandler");
    }

   private:
    Stream& stream_;
  };

  class WatchdogHandler {
   public:
    WatchdogHandler(Stream& stream): stream_(stream) {
    }

    void operator()(const Error& error) {
      if (!error) {
        return stream_.watchdog_handler();
      }
      if (error == boost::asio::error::operation_aborted) {
        // ignore this error (cancel on successful request)
        return;
      }
      throw std::runtime_error("WatchdogHandler");
    }

   private:
    Stream& stream_;
  };

  /**
    * @details If request/connection input parameters change,
    * response no longer corresponds to request, i.e. need to be cleared
    */
  void on_request_update();

  /**
    * @pre in_progress_ is false
    * @throw @c std::runtime_error
    */
  void check_not_in_progress();

  /**
    * @pre in_progress_ is true
    * @throw @c std::runtime_error
    */
  void check_in_progress();

  /**
    * @brief Check operation need to be stopped/restarted (by timeout or error)
    * @return false - resume current operation, true - return immediately
    * without starting new dependend handlers (operation will be restarted
    * automatically if needed)
    */
  bool stop_condition(const Error& error, const char* operation);
  void restart_operation();

  /**
    * @brief Start new operation or restart old
    * @note in_progress_ and force_stop_ flag is in valid state,
    * watchdog_timer_ already working
    */
  void start();

  // Handlers (methods)
  void resolve_handler(const Error& error, Resolver::iterator);
  void connect_handler(const Error& error, Resolver::iterator);
  void write_handler(const Error& error, std::size_t bytes_transferred);
  void read_some_handler(const Error& error, std::size_t bytes_transferred);
  void watchdog_handler();

  log::Logger log_;
  delegate::Interface* delegate_;
  boost::asio::ip::tcp::socket socket_;

  /**
    * @note HTTP-request pushed
    */
  bool in_progress_;

  /**
    * @note stop request (watchdog handler (e.g. by timeout) or error)
    */
  bool force_stop_;

  ConnectManager connect_manager_;

  Statistic statistic_;

  // Handlers (objects)
  ResolveHandler resolve_handler_;
  ConnectHandler connect_handler_;
  WriteHandler write_handler_;
  ReadSomeHandler read_some_handler_;
  RestartHandler restart_handler_;
  WatchdogHandler watchdog_handler_;
  ExtraSuccessHandler extra_success_handler_;

  boost::asio::deadline_timer restart_timer_;
  boost::asio::deadline_timer watchdog_timer_;
};

} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_STREAM_HPP_
