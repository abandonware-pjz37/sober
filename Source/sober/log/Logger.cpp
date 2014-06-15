// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/log/Logger.hpp>

#include <leathers/push>
#include <leathers/all>
# include <boost/log/attributes/constant.hpp>
# include <boost/log/sinks.hpp>
# include <boost/log/utility/setup.hpp>
# include <boost/utility/empty_deleter.hpp>
# include <fstream> // std::ofstream
#include <leathers/pop>

#include <sober/log/attributes.hpp> // sober::log::formatter
#include <sober/log/Severity.hpp>

namespace sober {
namespace log {

Logger::Logger(const char* name, const void* parent) {
  init(debug, name, Severity::DEBUG, parent);
  init(info, name, Severity::INFO, parent);
  init(error, name, Severity::ERROR, parent);
}

void Logger::init_logs_for_testing(bool log_to_cout, bool log_cout_verbose) {
  boost::log::add_common_attributes();
  boost::log::core::get()->remove_all_sinks();

  namespace sinks = boost::log::sinks;
  using Sink = sinks::synchronous_sink<sinks::text_ostream_backend>;

  auto sink_info = boost::make_shared<Sink>();
  sink_info->locked_backend()->add_stream(
      boost::make_shared<std::ofstream>("info.log")
  );
  if (log_to_cout) {
    sink_info->locked_backend()->add_stream(
        boost::shared_ptr<std::ostream>(&std::cout, boost::empty_deleter())
    );
  }
  sink_info->set_formatter(&log::formatter);
  if (!log_cout_verbose) {
    sink_info->set_filter(log::severity != log::Severity::DEBUG);
  }

  auto sink_debug = boost::make_shared<Sink>();
  sink_debug->locked_backend()->add_stream(
      boost::make_shared<std::ofstream>("debug.log")
  );
  sink_debug->set_formatter(&log::formatter);

  boost::log::core::get()->add_sink(sink_info);
  boost::log::core::get()->add_sink(sink_debug);
}

void Logger::init(
    Instance& log, const char* name, Severity sev, const void* parent
) {
  namespace attrs = boost::log::attributes;

  log.add_attribute(
      "sober.id",
      attrs::constant<std::string>(name)
  );

  log.add_attribute(
      "sober.severity",
      attrs::constant<Severity>(sev)
  );

  log.add_attribute(
      "sober.this",
      attrs::constant<const void*>(parent)
  );
}

} // namespace log
} // namespace sober
