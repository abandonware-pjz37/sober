#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_HTTPVERSION_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_HTTPVERSION_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/grammar/common.hpp> // qi

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// rfc7230, 2.6. Protocol Versioning
template <class Iterator>
struct HTTPVersion: qi::grammar<Iterator, void()> {
  using Base = qi::grammar<Iterator, void()>;

  HTTPVersion(): Base(start) {
    start = "HTTP/1.1";
  }

  qi::rule<Iterator, void()> start;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_HTTPVERSION_HPP_
