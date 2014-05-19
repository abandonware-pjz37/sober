// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/sink/String.hpp>

namespace sober {
namespace network {
namespace http {
namespace sink {

String::String() noexcept {
  clear();
}

void String::clear() noexcept {
  ready_ = false;
  value_.clear();
}

void String::write(char* buffer, std::size_t size, bool finish) {
  if (ready()) {
    throw std::runtime_error("Already finished");
  }
  body_.append(buffer, size);
  if (finish) {
    ready_ = true;
  }
}

bool String::ready() const noexcept {
  return ready_;
}

const std::string& String::body() const {
  if (!ready()) {
    throw std::runtime_error("Body not ready");
  }
  return body_;
}

} // namespace sink
} // namespace http
} // namespace network
} // namespace sober
