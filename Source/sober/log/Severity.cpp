// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/log/Severity.hpp>

#include <iostream> // std::ostream

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
#include <leathers/push>
#include <leathers/unreachable-code>
    default:
      throw std::runtime_error("sober::log::Severity switch");
#include <leathers/pop>
  }
  return stream;
}

} // namespace log
} // namespace sober
