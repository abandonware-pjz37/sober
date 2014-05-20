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

void Retry::on_start() {
  times_ = init_times_;
}

bool Retry::restart_on_error(const boost::system::error_code&) {
  return restart_on_error();
}

bool Retry::restart_on_error(const response::attribute::StatusCode&) {
  return restart_on_error();
}

boost::posix_time::time_duration Retry::restart_pause() {
  return pause_duration_;
}

bool Retry::restart_on_error() {
  if (times_ == 0) {
    return false;
  }

  --times_;
  return true;
}

} // namespace delegate
} // namespace http
} // namespace network
} // namespace sober
