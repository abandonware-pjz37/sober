// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/delegate/Interface.hpp>

#include <leathers/push>
# include <leathers/all>
# include <boost/concept_check.hpp> // ignore_unused_variable_warning
#include <leathers/pop>

namespace sober {
namespace network {
namespace http {
namespace delegate {

void Interface::on_start() {
}

Interface::Iterator Interface::pick_connect_endpoint(Iterator iterator) {
  return iterator;
}

bool Interface::allow_redirection() {
  return false;
}

void Interface::on_success() {
}

void Interface::body_start() {
}

void Interface::body_write(const char* buffer, std::size_t size) {
  boost::ignore_unused_variable_warning(buffer);
  boost::ignore_unused_variable_warning(size);
}

void Interface::body_finish() {
}

boost::posix_time::time_duration Interface::watchdog_period() {
  return boost::posix_time::pos_infin;
}

bool Interface::force_stop() {
  return false;
}

bool Interface::force_reconnect() {
  return false;
}

boost::posix_time::time_duration Interface::restart_pause() {
  return boost::posix_time::milliseconds(0);
}

bool Interface::restart_on_error(const boost::system::error_code&) {
  return false;
}

bool Interface::restart_on_error(const response::attribute::StatusCode&) {
  return false;
}

Interface::~Interface() {
}

} // namespace delegate
} // namespace http
} // namespace network
} // namespace sober
