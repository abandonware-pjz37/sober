// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <iomanip> // std::setprecision
#include <sober/network/api/OpenWeatherMap.hpp>
#include <sober/network/http/Stream.hpp>

namespace sober {
namespace network {
namespace api {

OpenWeatherMap::OpenWeatherMap(http::Stream& stream): stream_(stream) {
  stream_.set_endpoint("http://api.openweathermap.org/data/2.5/weather");
  stream_.set_delegate(*this);
}

void OpenWeatherMap::async_get_city(const char* city) {
  stream_.request.clear_query();
  stream_.request.add_query("q", city);
  stream_.request.add_query("units", "metric");
  stream_.async_start();
}

void OpenWeatherMap::async_get_city(const std::string& city) {
  async_get_city(city.c_str());
}

void OpenWeatherMap::Attribute::fill(const ciere::json::value& value) {
  namespace json = ciere::json;

  if (value["cod"].get_as<json::int_t>() == 404) {
    std::string message = value["message"];
    throw std::runtime_error(message);
  }

  const ciere::json::value& coord = value["coord"];
  const ciere::json::value& weather = value["weather"][0];

  // Warning:
  //   if `lon`/`lat` is integer (i.e. 12, but not 12.0),
  //   json::value::get<double_t> will throw exception,
  //   get_as<double_t> need to be used.
  longitude = coord["lon"].get_as<json::double_t>();
  latitude = coord["lat"].get_as<json::double_t>();
  temperature = value["main"]["temp"].get_as<json::double_t>();
  description = weather["description"].get<json::string_t>();
  icon = weather["icon"].get<json::string_t>();

  str_.str("");
  if (temperature > 0) {
    str_ << "+";
  }
  str_ << std::setprecision(1) << std::fixed << temperature << " ";
  str_ << "\u2103"; // Unicode: DEGREE CELSIUS

  temperature_human = str_.str();
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
