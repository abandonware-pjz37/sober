#ifndef SOBER_NETWORK_HTTP_DELEGATE_INTERFACE_HPP_
#define SOBER_NETWORK_HTTP_DELEGATE_INTERFACE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/delegate/Interface.fpp>

#include <boost/asio/ip/tcp.hpp>
#include <sober/network/http/response/attribute/StatusCode.fpp>

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
  virtual void on_start() {
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

  //@{
  /**
    * @details Do restart if error occurs
    * @note Called when error occurs
    */
  virtual bool restart_on_error(const boost::system::error_code&) {
    return false;
  }

  virtual bool restart_on_error(const response::attribute::StatusCode&) {
    return false;
  }
  //@}

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
    * @details Successfull HTTP-response event:
    * 1. body is ready
    * 2. status code is OK
    */
  virtual void on_success() {
  }
};

} // namespace delegate
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_DELEGATE_INTERFACE_HPP_
