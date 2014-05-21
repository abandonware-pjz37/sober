// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/api/OpenWeatherMap.hpp>

namespace sober {
namespace network {
namespace api {

void OpenWeatherMap::Attribute::fill(const ciere::json::value& value) {
  namespace json = ciere::json;

  const ciere::json::value& coord = value["coord"];
  const ciere::json::value& weather = value["weather"][0];

  longitude = coord["lon"].get<json::double_t>();
  latitude = coord["lat"].get<json::double_t>();
  description = weather["description"].get<json::string_t>();
  description = weather["icon"].get<json::string_t>();
}

void OpenWeatherMap::body_start() noexcept {
  Base::body_start();
  valid_ = false;
}

void OpenWeatherMap::on_success() {
  Base::on_success();
  attribute_.fill(Base::json());
  valid_ = true;
}

const OpenWeatherMap::Attribute& OpenWeatherMap::attribute() const noexcept {
  if (!valid_) {
    throw std::runtime_error("OpenWeatherMap attribute not valid");
  }
  return attribute_;
}

} // namespace api
} // namespace network
} // namespace sober
