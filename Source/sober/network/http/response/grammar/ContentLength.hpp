#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CONTENTLENGTH_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CONTENTLENGTH_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/grammar/common.hpp> // qi
#include <sober/network/http/response/attribute/ContentLength.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// rfc7230, 3.3.2. Content-Length
template <class Iterator>
struct ContentLength: qi::grammar<Iterator, attribute::ContentLength()> {
  using Base = qi::grammar<Iterator, response::attribute::ContentLength()>;

  ContentLength(): Base(content_length) {
    content_length %= qi::lit("Content-Length: ") >> uint_parser;
  }

  qi::uint_parser<unsigned, 10, 1, -1> uint_parser;
  qi::rule<Iterator, response::attribute::ContentLength()> content_length;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CONTENTLENGTH_HPP_
