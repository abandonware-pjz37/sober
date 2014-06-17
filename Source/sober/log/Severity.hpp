#ifndef SOBER_LOG_SEVERITY_HPP_
#define SOBER_LOG_SEVERITY_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/log/Severity.fpp>

#include <iosfwd> // std::ostream

#undef ERROR // Hello, visual studio...

namespace sober {
namespace log {

enum class Severity {
  DEBUG, // All messages
  INFO, // Informational
  ERROR // Detailed description of thrown exception
};

std::ostream& operator<<(std::ostream& stream, Severity severity);

} // namespace log
} // namespace sober


#endif // SOBER_LOG_SEVERITY_HPP_
