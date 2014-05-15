// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/Statistic.hpp>

namespace sober {
namespace network {
namespace http {

Statistic::Statistic() noexcept {
  clear();
}

void Statistic::clear() noexcept {
  reconnect_ = 0;
  restarted_ = 0;
}

void Statistic::on_reconnect() noexcept {
  ++reconnect_;
}

void Statistic::on_restart() noexcept {
  ++restarted_;
}

Statistic::Counter Statistic::get_reconnect() const noexcept {
  return reconnect_;
}

Statistic::Counter Statistic::get_restarted() const noexcept {
  return restarted_;
}

} // namespace http
} // namespace network
} // namespace sober
