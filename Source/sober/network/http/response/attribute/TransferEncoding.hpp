#ifndef SOBER_NETWORK_HTTP_RESPONSE_ATTRIBUTE_TRANSFERENCODING_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_ATTRIBUTE_TRANSFERENCODING_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/attribute/StatusLine.hpp>
#include <sober/network/http/response/attribute/ContentLength.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace attribute {

enum class TransferEncoding {
  CHUNKED,
  OTHER
};

} // namespace attribute
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_ATTRIBUTE_TRANSFERENCODING_HPP_
