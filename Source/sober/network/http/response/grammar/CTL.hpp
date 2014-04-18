#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CTL_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CTL_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/grammar/common.hpp> // qi

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// 2.2 Basic Rules
template <class Iterator>
struct CTL: qi::grammar<Iterator, void()> {
  using Base = qi::grammar<Iterator, void()>;

  CTL(): Base(ctl) {
    // any US-ASCII control character (octets 0 - 31) and DEL (127)
    ctl = qi::ascii::char_(0, 31) | qi::ascii::char_(127);
  }

  qi::rule<Iterator, void()> ctl;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CTL_HPP_
