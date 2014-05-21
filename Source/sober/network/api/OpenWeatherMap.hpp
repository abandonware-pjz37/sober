#ifndef SOBER_NETWORK_API_OPENWEATHERMAP_HPP_
#define SOBER_NETWORK_API_OPENWEATHERMAP_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/delegate/Json.hpp>

namespace sober {
namespace network {
namespace api {

class OpenWeatherMap : public http::delegate::Json {
 public:
  using Base = http::delegate::Json;

  struct Attribute {
    double longitude;
    double latitude;
    std::string description;
    std::string icon;

    void fill(const ciere::json::value&);
  };

  virtual void body_start() noexcept override;
  virtual void on_success() override;

  const Attribute& attribute() const noexcept;

 private:
  bool valid_;
  Attribute attribute_;
};

} // namespace api
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_API_OPENWEATHERMAP_HPP_
