#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_VCHAR_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_VCHAR_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/grammar/common.hpp> // qi
#include <sober/network/http/response/attribute/VCHAR.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// rfc5234, B.1. Core Rules
template <class Iterator>
struct VCHAR: qi::grammar<Iterator, attribute::VCHAR()> {
  using Base = qi::grammar<Iterator, response::attribute::VCHAR()>;

  VCHAR(): Base(vchar) {
    vchar %= qi::ascii::char_(0x21, 0x7E);
  }

  qi::rule<Iterator, response::attribute::VCHAR()> vchar;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_VCHAR_HPP_
