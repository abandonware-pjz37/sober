#ifndef SOBER_NETWORK_HTTP_STREAM_HPP_
#define SOBER_NETWORK_HTTP_STREAM_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/Stream.fpp>

#include <boost/asio.hpp>
#include <sober/log/Logger.hpp>
#include <sober/network/Engine.fpp>
#include <sober/network/http/ConnectManager.hpp>
#include <sober/network/http/Statistic.hpp>
#include <sober/network/http/delegate/Interface.fpp>
#include <sober/network/http/Request.hpp>
#include <sober/network/http/Response.hpp>

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
  * Stream stream(engine);
  * // 2. Config
  * stream.set_endpoint(...);
  * stream.set_method(...); // opt
  * stream.set_path(...); // opt
  * stream.set_query(...); // opt
  * // 3. Push request
  * stream.push_request(...);
  * // 4. Process
  * engine.run();
  * // 5. Response
  * if (stream.response_valid()) {
  *   auto message = stream.response_body();
  * }
  * @endcode
  */
class Stream {
 public:
  using Error = boost::system::error_code;

  Stream(Engine&, Request& request, Response& response);

  Stream(Stream&&) = delete;
  Stream& operator=(Stream&&) = delete;

  Stream(const Stream&) = delete;
  Stream& operator=(const Stream&) = delete;

  //@{
  /**
    * @note Set both @c ConnectManager and @c Request
    * @brief Set host and port to @c ConnectManager
    * @brief Set host and query to @c Request
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
  void async_start();

  /**
    * @brief Cancel current operation
    */
  void cancel();

  //@{
  /**
    * @brief HTTP-stream statistic
    */
  const Statistic& statistic() const noexcept;
  void clear_statistic() noexcept;
  //@}

  const char* log_name() const noexcept;

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

  log::Logger log_debug_;
  log::Logger log_info_;
  log::Logger log_error_;
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

  Request& request_;
  Response& response_;

  ConnectManager connect_manager_;

  Statistic statistic_;

  // Handlers (objects)
  ResolveHandler resolve_handler_;
  ConnectHandler connect_handler_;
  WriteHandler write_handler_;
  ReadSomeHandler read_some_handler_;
  RestartHandler restart_handler_;
  WatchdogHandler watchdog_handler_;

  boost::asio::deadline_timer restart_timer_;
  boost::asio::deadline_timer watchdog_timer_;
};

} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_STREAM_HPP_
