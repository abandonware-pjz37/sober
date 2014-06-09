#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CRLF_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CRLF_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/grammar/CR.hpp>
#include <sober/network/http/response/grammar/LF.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// rfc5234, B.1. Core Rules
template <class Iterator>
struct CRLF: qi::grammar<Iterator, void()> {
  using Base = qi::grammar<Iterator, void()>;

  CRLF(): Base(crlf) {
    crlf = cr >> lf;
  }

  CR<Iterator> cr;
  LF<Iterator> lf;

  qi::rule<Iterator, void()> crlf;
};


} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CRLF_HPP_
