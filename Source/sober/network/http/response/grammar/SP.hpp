#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_SP_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_SP_HPP_

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
struct SP: qi::grammar<Iterator, void()> {
  using Base = qi::grammar<Iterator, void()>;

  SP(): Base(sp) {
    sp = ' ';
  }

  qi::rule<Iterator, void()> sp;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_SP_HPP_
