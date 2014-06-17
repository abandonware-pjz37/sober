#ifndef SOBER_NETWORK_HTTP_STATISTIC_HPP_
#define SOBER_NETWORK_HTTP_STATISTIC_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <leathers/push>
#include <leathers/all>
# include <cstdint> // uint64_t
# include <boost/config.hpp> // BOOST_NOEXCEPT
#include <leathers/pop>

namespace sober {
namespace network {
namespace http {

class Statistic {
 public:
  using Counter = uint64_t;

  Statistic() BOOST_NOEXCEPT;

  void clear() BOOST_NOEXCEPT;

  void on_reconnect() BOOST_NOEXCEPT;
  void on_restart() BOOST_NOEXCEPT;

  Counter get_reconnect() const BOOST_NOEXCEPT;
  Counter get_restarted() const BOOST_NOEXCEPT;

 private:
  Counter reconnect_;
  Counter restarted_;
};

} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_STATISTIC_HPP_
