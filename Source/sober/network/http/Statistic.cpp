// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/Statistic.hpp>

namespace sober {
namespace network {
namespace http {

Statistic::Statistic() BOOST_NOEXCEPT {
  clear();
}

void Statistic::clear() BOOST_NOEXCEPT {
  reconnect_ = 0;
  restarted_ = 0;
}

void Statistic::on_reconnect() BOOST_NOEXCEPT {
  ++reconnect_;
}

void Statistic::on_restart() BOOST_NOEXCEPT {
  ++restarted_;
}

Statistic::Counter Statistic::get_reconnect() const BOOST_NOEXCEPT {
  return reconnect_;
}

Statistic::Counter Statistic::get_restarted() const BOOST_NOEXCEPT {
  return restarted_;
}

} // namespace http
} // namespace network
} // namespace sober
