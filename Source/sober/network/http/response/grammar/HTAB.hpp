#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_HTAB_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_HTAB_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/grammar/common.hpp> // qi
#include <sober/network/http/response/attribute/HTAB.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// rfc5234, B.1. Core Rules
template <class Iterator>
struct HTAB: qi::grammar<Iterator, attribute::HTAB()> {
  using Base = qi::grammar<Iterator, response::attribute::HTAB()>;

  HTAB(): Base(htab) {
    htab %= qi::ascii::char_(0x09);
  }

  qi::rule<Iterator, response::attribute::HTAB()> htab;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_HTAB_HPP_
