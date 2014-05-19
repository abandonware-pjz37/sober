// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/delegate/Reconnect.hpp>

namespace sober {
namespace network {
namespace http {
namespace delegate {

Reconnect::Reconnect(Duration period): period_(period) {
}

void Reconnect::on_start() {
  current_ = Iterator();
  force_reconnect_ = true;
}

bool Reconnect::force_reconnect() {
  return force_reconnect_;
}

Reconnect::Iterator Reconnect::pick_connect_endpoint(Iterator iterator) {
  Iterator check_size_it = iterator;
  ++check_size_it;

  if (check_size_it == Iterator()) {
    // There is only one iterator, reconnection doesn't make any sense
    force_reconnect_ = false;
    return iterator;
  }
  else {
    force_reconnect_ = true;
  }

  if (current_ != Iterator()) {
    ++current_;
  }
  if (current_ == Iterator()) {
    current_ = iterator;
  }
  return current_;
}

boost::posix_time::time_duration Reconnect::watchdog_period() {
  return period_;
}

} // namespace delegate
} // namespace http
} // namespace network
} // namespace sober
