#ifndef SOBER_NETWORK_HTTP_DELEGATE_JSON_HPP_
#define SOBER_NETWORK_HTTP_DELEGATE_JSON_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <leathers/push>
#include <leathers/all>
# include <ciere/json/value.hpp>
#include <leathers/pop>

#include <sober/log/Logger.hpp>
#include <sober/network/http/delegate/String.hpp>
#include <string>

namespace sober {
namespace network {
namespace http {
namespace delegate {

class Json: public String {
 public:
  using Base = String;

  Json();

  virtual void body_start() BOOST_NOEXCEPT override;

  /**
    * @throws @c cierre::json::exception if JSON parse failed
    */
  virtual void on_success() override;

  /**
    * @throws @c std::runtime_error if body not ready
    */
  const ciere::json::value& json() const;

 private:
  log::Logger log_;
  bool valid_;
  ciere::json::value json_;
};

} // namespace delegate
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_DELEGATE_JSON_HPP_
