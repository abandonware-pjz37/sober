#ifndef SOBER_NETWORK_HTTP_DELEGATE_INTERFACE_HPP_
#define SOBER_NETWORK_HTTP_DELEGATE_INTERFACE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/delegate/Interface.fpp>

#include <boost/asio/ip/tcp.hpp>

namespace sober {
namespace network {
namespace http {
namespace delegate {

class Interface {
 public:
  using Iterator = boost::asio::ip::tcp::resolver::iterator;

  /**
    * @note On operation start. Init internal state.
    */
  virtual void on_push_request() {
  }

  /**
    * @details Do stop on watchdog callback
    * @note Called when watchdog timer triggered
    */
  virtual bool force_stop() {
    return false;
  }

  /**
    * @details Try other connection if not already connected
    * @note Called when watchdog timer triggered
    */
  virtual bool force_reconnect() {
    return false;
  }

  /**
    * @details User can specify to which endpoint connect first
    * @note Called on initialization of asynchronous connect operation
    */
  virtual Iterator pick_connect_endpoint(Iterator iterator) {
    return iterator;
  }

  /**
    * @details Do restart if error occurs
    * @note Called when error occurs
    */
  virtual bool restart_on_error() {
    return false;
  }

  /**
    * @details Watchdog callback period
    */
  virtual boost::posix_time::time_duration watchdog_period() {
    return boost::posix_time::pos_infin;
  }

  /**
    * @details Duration of pause to restart operation
    * @note Called after restart_on_error (if true returned)
    */
  virtual boost::posix_time::time_duration restart_pause() {
    return boost::posix_time::milliseconds(0);
  }

  /**
    * @details Handler with reference to body of successfull HTTP-response
    * @warning body is a reference to @c Stream internals. Effectively
    * this means that if @c Stream instance will be used, then body can
    * change his value; For example: if reference to body will be saved and
    * other @method Stream::push_request operation will be called
    * (and processed), then the reference will no longer point to correct
    * body value.
    */
  virtual void on_success(const std::string& body) {
  }
};

} // namespace delegate
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_DELEGATE_INTERFACE_HPP_
