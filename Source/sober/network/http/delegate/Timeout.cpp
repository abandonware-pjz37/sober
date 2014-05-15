// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/delegate/Timeout.hpp>

namespace sober {
namespace network {
namespace http {
namespace delegate {

Timeout::Timeout(Duration timeout): timeout_(timeout) {
}

bool Timeout::force_stop() {
  return true;
}

boost::posix_time::time_duration Timeout::watchdog_period() {
  return timeout_;
}

} // namespace delegate
} // namespace http
} // namespace network
} // namespace sober
