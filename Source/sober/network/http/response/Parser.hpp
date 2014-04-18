#ifndef SOBER_NETWORK_HTTP_RESPONSE_PARSER_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_PARSER_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

namespace sober {
namespace network {
namespace http {
namespace response {

class Parser {
 public:
  template <class Iterator>
  static bool parse(Iterator begin, Iterator end, std::string& body);
};

} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_PARSER_HPP_
