#ifndef SOBER_NETWORK_HTTP_STATISTIC_HPP_
#define SOBER_NETWORK_HTTP_STATISTIC_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <cstdint> // uint64_t

namespace sober {
namespace network {
namespace http {

class Statistic {
 public:
  using Counter = uint64_t;

  Statistic() noexcept;

  void clear() noexcept;

  void on_reconnect() noexcept;
  void on_restart() noexcept;

  Counter get_reconnect() const noexcept;
  Counter get_restarted() const noexcept;

 private:
  Counter reconnect_;
  Counter restarted_;
};

} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_STATISTIC_HPP_
