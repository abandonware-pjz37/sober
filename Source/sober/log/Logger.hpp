#ifndef SOBER_LOG_LOGGER_HPP_
#define SOBER_LOG_LOGGER_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <boost/log/sources/logger.hpp>
#include <memory> // std::addressof
#include <sober/log/Severity.fpp>

namespace sober {
namespace log {

class Logger: public boost::log::sources::logger {
 public:
  using Base = boost::log::sources::logger;

  template <class Parent>
  Logger(const Parent& parent, Severity severity):
      Logger(parent.log_name(), severity, std::addressof(parent)) {
  }

 private:
  Logger(const char* name, Severity, const void* parent);
};

} // namespace log
} // namespace sober

#endif // SOBER_LOG_LOGGER_HPP_
