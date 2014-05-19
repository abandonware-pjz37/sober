#ifndef SOBER_NETWORK_HTTP_DELEGATE_RECONNECT_HPP_
#define SOBER_NETWORK_HTTP_DELEGATE_RECONNECT_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/delegate/Interface.hpp>

namespace sober {
namespace network {
namespace http {
namespace delegate {

class Reconnect: virtual public Interface {
 public:
  using Duration = boost::posix_time::time_duration;

  Reconnect(Duration period);

  virtual void on_start() override;
  virtual bool force_reconnect() override;
  virtual Iterator pick_connect_endpoint(Iterator iterator) override;
  virtual boost::posix_time::time_duration watchdog_period() override;

 private:
  const Duration period_;
  bool force_reconnect_;
  Iterator current_;
};

} // namespace delegate
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_DELEGATE_RECONNECT_HPP_
