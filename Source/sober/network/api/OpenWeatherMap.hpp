#ifndef SOBER_NETWORK_API_OPENWEATHERMAP_HPP_
#define SOBER_NETWORK_API_OPENWEATHERMAP_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/ExtraSuccessHandler.hpp>
#include <sober/network/http/Stream.fpp>
#include <sober/network/http/delegate/Json.hpp>

namespace sober {
namespace network {
namespace api {

// http://bugs.openweathermap.org/projects/api/wiki/Weather_Data
class OpenWeatherMap : public http::delegate::Json {
 public:
  using Base = http::delegate::Json;

  OpenWeatherMap(http::Stream&);

  struct Attribute {
    double longitude;
    double latitude;
    double temperature;
    std::string temperature_human;
    std::string description;
    std::string icon;

    void fill(const ciere::json::value&);

   private:
    std::ostringstream str_;
  };

  void async_get_city(
      const char* city, http::ExtraSuccessHandler handler = nullptr
  );
  void async_get_city(
      const std::string& city, http::ExtraSuccessHandler handler = nullptr
  );

  virtual void body_start() BOOST_NOEXCEPT override;
  virtual void on_success() override;

  /**
    * @throw @c std::runtime_error if attribute not valid
    */
  const Attribute& attribute() const;

 private:
  http::Stream& stream_;

  bool valid_;
  Attribute attribute_;
};

} // namespace api
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_API_OPENWEATHERMAP_HPP_
