#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_SP_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_SP_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/grammar/common.hpp> // qi
#include <sober/network/http/response/attribute/SP.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// rfc5234, B.1. Core Rules
template <class Iterator>
struct SP: qi::grammar<Iterator, attribute::SP()> {
  using Base = qi::grammar<Iterator, response::attribute::SP()>;

  SP(): Base(sp) {
    sp %= ' ';
  }

  qi::rule<Iterator, response::attribute::SP()> sp;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_SP_HPP_
