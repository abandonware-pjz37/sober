// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/log/Logger.hpp>

#include <boost/log/attributes/constant.hpp>
#include <sober/log/Severity.hpp>

namespace sober {
namespace log {

Logger::Logger(const char* name, Severity severity, const void* parent) {
  namespace attrs = boost::log::attributes;

  Base::add_attribute(
      "sober.id",
      attrs::constant<std::string>(name)
  );

  Base::add_attribute(
      "sober.severity",
      attrs::constant<Severity>(severity)
  );

  Base::add_attribute(
      "sober.this",
      attrs::constant<const void*>(parent)
  );
}

} // namespace log
} // namespace sober
