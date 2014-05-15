// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/delegate/Retry.hpp>

namespace sober {
namespace network {
namespace http {
namespace delegate {

Retry::Retry(unsigned times, Duration pause_duration):
    init_times_(times),
    pause_duration_(pause_duration),
    times_(init_times_) {
}

void Retry::on_push_request() {
  times_ = init_times_;
}

bool Retry::restart_on_error() {
  if (times_ == 0) {
    return false;
  }

  --times_;
  return true;
}

boost::posix_time::time_duration Retry::restart_pause() {
  return pause_duration_;
}

} // namespace delegate
} // namespace http
} // namespace network
} // namespace sober
