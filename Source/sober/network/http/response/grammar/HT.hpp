#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_HT_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_HT_HPP_

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
struct HT: qi::grammar<Iterator, void()> {
  using Base = qi::grammar<Iterator, void()>;

  HT(): Base(ht) {
    // ASCII code 9: Horizontal Tab
    ht = qi::ascii::char_(9);
  }

  qi::rule<Iterator, void()> ht;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_HT_HPP_
