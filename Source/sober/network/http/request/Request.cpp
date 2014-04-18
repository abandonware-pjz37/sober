// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/request/Request.hpp>

namespace sober {
namespace network {
namespace http {
namespace request {

Request Request::make_get(const std::string& host_name) {
  return make_1_1("GET", host_name.c_str());
}

const std::string& Request::str() const noexcept {
  return request_;
}

Request Request::make_1_1(const char* method, const std::string& host_name) {
  std::string result(method);
  result += " / HTTP/1.1\r\n";
  result += ("Host: " + host_name + "\r\n\r\n");

  Request request;
  request.request_ = std::move(result);
  return request;
}

} // namespace request
} // namespace http
} // namespace network
} // namespace sober
