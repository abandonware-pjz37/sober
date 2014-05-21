// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/delegate/Json.hpp>

#include <ciere/json/io.hpp> // json::construct

namespace sober {
namespace network {
namespace http {
namespace delegate {

void Json::body_start() noexcept {
  Base::body_start();
  valid_ = false;
}

void Json::on_success() {
  Base::on_success();
  ciere::json::construct(body().c_str(), json_);
  valid_ = true;
}

const ciere::json::value& Json::json() const {
  if (!valid_) {
    throw std::runtime_error("Json not valid");
  }
  return json_;
}

} // namespace delegate
} // namespace http
} // namespace network
} // namespace sober
