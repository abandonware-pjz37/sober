// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/delegate/String.hpp>

#include <exception> // std::runtime_error

namespace sober {
namespace network {
namespace http {
namespace delegate {

String::String() BOOST_NOEXCEPT: ready_(false) {
}

void String::body_start() BOOST_NOEXCEPT {
  ready_ = false;
  body_.clear();
}

void String::body_write(const char* buffer, std::size_t size) {
  if (ready()) {
    throw std::runtime_error("Already finished");
  }
  body_.append(buffer, size);
}

void String::body_finish() {
  if (ready()) {
    throw std::runtime_error("Unexpected body_finish");
  }
  ready_ = true;
}

bool String::ready() const BOOST_NOEXCEPT {
  return ready_;
}

const std::string& String::body() const {
  if (!ready()) {
    throw std::runtime_error("Body not ready");
  }
  return body_;
}

} // namespace delegate
} // namespace http
} // namespace network
} // namespace sober
