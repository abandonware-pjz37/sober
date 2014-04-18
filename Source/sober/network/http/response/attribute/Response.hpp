#ifndef SOBER_NETWORK_HTTP_RESPONSE_ATTRIBUTE_RESPONSE_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_ATTRIBUTE_RESPONSE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/attribute/StatusLine.hpp>
#include <sober/network/http/response/attribute/ContentLength.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace attribute {

struct Response {
  StatusLine status_line;
  ContentLength content_length;
  std::string message_body;
};

} // namespace attribute
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

BOOST_FUSION_ADAPT_STRUCT(
    sober::network::http::response::attribute::Response,
    (sober::network::http::response::attribute::StatusLine, status_line)
    (sober::network::http::response::attribute::ContentLength, content_length)
    (std::string, message_body)
)

#endif // SOBER_NETWORK_HTTP_RESPONSE_ATTRIBUTE_RESPONSE_HPP_
