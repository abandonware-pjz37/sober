// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/delegate/Json.hpp>

#include <leathers/push>
#include <leathers/all>
# include <boost/log/sources/record_ostream.hpp> // BOOST_LOG
# include <ciere/json/io.hpp> // json::construct
#include <leathers/pop>

namespace sober {
namespace network {
namespace http {
namespace delegate {

Json::Json(): log_("sober.network.http.delegate.Json", this) {
}

void Json::body_start() BOOST_NOEXCEPT {
  Base::body_start();
  valid_ = false;
}

void Json::on_success() {
  Base::on_success();
  BOOST_LOG(log_.debug) << "Construct JSON from:" << std::endl << body();
  const bool ok = ciere::json::construct(body(), json_);
  if (!ok) {
    throw std::runtime_error("JSON parse failed");
  }
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
