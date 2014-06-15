// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/request/Method.hpp>

#include <stdexcept> // std::runtime_error

namespace sober {
namespace network {
namespace http {
namespace request {

const char* to_const_char(const Method& method) {
  switch(method) {
    case Method::OPTIONS: return "OPTIONS";
    case Method::GET: return "GET";
    case Method::HEAD: return "HEAD";
    case Method::POST: return "POST";
    case Method::PUT: return "PUT";
    case Method::DELETE: return "DELETE";
    case Method::TRACE: return "TRACE";
    case Method::CONNECT: return "CONNECT";
#include <leathers/push>
#include <leathers/unreachable-code>
    default:
      throw std::runtime_error("Can't print Method");
#include <leathers/pop>
  }
}

} // namespace request
} // namespace http
} // namespace network
} // namespace sober
