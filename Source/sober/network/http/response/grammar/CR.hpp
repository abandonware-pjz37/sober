#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CR_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CR_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/grammar/common.hpp> // qi

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// rfc5234, B.1. Core Rules
template <class Iterator>
struct CR: qi::grammar<Iterator, void()> {
  using Base = qi::grammar<Iterator, void()>;

  CR(): Base(cr) {
    cr = qi::char_('\r');
  }

  qi::rule<Iterator, void()> cr;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CR_HPP_
