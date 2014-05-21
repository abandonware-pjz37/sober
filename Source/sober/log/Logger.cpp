// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/log/Logger.hpp>

#include <boost/log/attributes/constant.hpp>
#include <sober/log/Severity.hpp>

namespace sober {
namespace log {

Logger::Logger(const char* name, const void* parent) {
  init(debug, name, Severity::DEBUG, parent);
  init(info, name, Severity::INFO, parent);
  init(error, name, Severity::ERROR, parent);
}

void Logger::init(
    Instance& log, const char* name, Severity severity, const void* parent
) {
  namespace attrs = boost::log::attributes;

  log.add_attribute(
      "sober.id",
      attrs::constant<std::string>(name)
  );

  log.add_attribute(
      "sober.severity",
      attrs::constant<Severity>(severity)
  );

  log.add_attribute(
      "sober.this",
      attrs::constant<const void*>(parent)
  );
}

} // namespace log
} // namespace sober