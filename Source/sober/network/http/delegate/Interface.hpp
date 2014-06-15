#ifndef SOBER_NETWORK_HTTP_DELEGATE_INTERFACE_HPP_
#define SOBER_NETWORK_HTTP_DELEGATE_INTERFACE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/delegate/Interface.fpp>

#include <leathers/push>
#include <leathers/all>
# include <boost/asio/ip/tcp.hpp>
#include <leathers/pop>

#include <sober/network/http/response/attribute/StatusCode.fpp>

namespace sober {
namespace network {
namespace http {
namespace delegate {

class Interface {
 public:
  using Iterator = boost::asio::ip::tcp::resolver::iterator;

  /**
    * @note On operation start. Init internal state (Stream::async_start)
    */
  virtual void on_start();

  /**
    * @details User can specify to which endpoint connect first
    * @note Called on initialization of asynchronous connect operation
    */
  virtual Iterator pick_connect_endpoint(Iterator iterator);

  /**
    * @details Allow URL redirection
    */
  virtual bool allow_redirection();

  /**
    * @details Successfull HTTP-response event:
    * 1. body is ready
    * 2. status code is OK
    */
  virtual void on_success();

  /**
    * @defgroup HTTP-response body
    * @{
    */
  /**
    * @details Start writing body
    */
  virtual void body_start();

  /**
    * @details Body data chunk
    */
  virtual void body_write(const char* buffer, std::size_t size);

  /**
    * @details Finish writing body
    * @note don't mean response is successful (check status code)
    */
  virtual void body_finish();
  /** @} */

  /**
    * @defgroup Watchdog events
    * @{
    */

  /**
    * @details Watchdog callback period
    */
  virtual boost::posix_time::time_duration watchdog_period();

  /**
    * @details Do stop on watchdog callback
    */
  virtual bool force_stop();

  /**
    * @details Try other connection if not already connected
    */
  virtual bool force_reconnect();

  /** @} */

  /**
    * @defgroup Error events
    * @{
    */

  /**
    * @details Duration of pause to restart operation
    */
  virtual boost::posix_time::time_duration restart_pause();

  /**
    * @details Do restart if error occurs
    */
  virtual bool restart_on_error(const boost::system::error_code&);

  /**
    * @details Do restart if error occurs (status code is not OK)
    */
  virtual bool restart_on_error(const response::attribute::StatusCode&);
  /** @} */

  virtual ~Interface();
};

} // namespace delegate
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_DELEGATE_INTERFACE_HPP_
