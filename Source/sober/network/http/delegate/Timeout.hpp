#ifndef SOBER_NETWORK_HTTP_DELEGATE_TIMEOUT_HPP_
#define SOBER_NETWORK_HTTP_DELEGATE_TIMEOUT_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/delegate/Interface.hpp>

namespace sober {
namespace network {
namespace http {
namespace delegate {

class Timeout: virtual public Interface {
 public:
  using Duration = boost::posix_time::time_duration;

  Timeout(Duration timeout);

  virtual bool force_stop() override;
  virtual boost::posix_time::time_duration watchdog_period() override;

 private:
  const Duration timeout_;
};

} // namespace delegate
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_DELEGATE_TIMEOUT_HPP_
