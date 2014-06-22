// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/log/Severity.hpp>

#include <iostream> // std::ostream
#include <stdexcept> // std::runtime_error

namespace sober {
namespace log {

std::ostream& operator<<(std::ostream& stream, Severity severity) {
  switch (severity) {
    case Severity::DEBUG:
      stream << "DEBUG";
      break;
    case Severity::INFO:
      stream << "INFO";
      break;
    case Severity::ERROR:
      stream << "ERROR";
      break;
    default:
      throw std::runtime_error("sober::log::Severity switch");
  }
  return stream;
}

} // namespace log
} // namespace sober
