#ifndef SOBER_NETWORK_TCP_HTTP_REQUEST_METHOD_HPP_
#define SOBER_NETWORK_TCP_HTTP_REQUEST_METHOD_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#undef DELETE // Hello, Visual Studio...

namespace sober {
namespace network {
namespace http {
namespace request {

// http://tools.ietf.org/html/rfc7231#section-4.3
enum class Method {
  OPTIONS,
  GET,
  HEAD,
  POST,
  PUT,
  DELETE,
  TRACE,
  CONNECT
};

const char* to_const_char(const Method& method);

} // namespace request
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_TCP_HTTP_REQUEST_METHOD_HPP_
