// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/utils/Test.hpp>

#include <boost/log/sinks.hpp>
#include <boost/log/utility/empty_deleter.hpp>
#include <boost/log/utility/setup.hpp>
#include <fstream> // std::ofstream
#include <sober/log/attributes.hpp> // sober::log::formatter

namespace sober {
namespace utils {

void Test::init_logs(bool log_to_cout, bool log_cout_verbose) {
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
        boost::shared_ptr<std::ostream>(&std::cout, boost::log::empty_deleter())
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

void Test::SetUpTestCase() {
  Base::SetUpTestCase();

  const bool log_to_cout = false;
  const bool log_cout_verbose = false;

  init_logs(log_to_cout, log_cout_verbose);
}

} // namespace utils
} // namespace sober
