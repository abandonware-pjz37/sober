#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_STATUSCODE_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_STATUSCODE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/grammar/common.hpp> // qi
#include <sober/network/http/response/attribute/StatusCode.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// rfc7230, 3.1.2. Status Line
template <class Iterator>
struct StatusCode: qi::grammar<Iterator, attribute::StatusCode()> {
  using Base = qi::grammar<Iterator, response::attribute::StatusCode()>;

  StatusCode(): Base(status_code) {
    status_code %= uint_parser;
  }

  qi::uint_parser<unsigned, 10, 3, 3> uint_parser;
  qi::rule<Iterator, response::attribute::StatusCode()> status_code;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_STATUSCODE_HPP_
