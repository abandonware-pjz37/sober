#ifndef SOBER_LOG_LOGGER_HPP_
#define SOBER_LOG_LOGGER_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <leathers/push>
#include <leathers/all>
# include <boost/log/sources/logger.hpp>
# include <sober/log/Severity.fpp>
#include <leathers/pop>

namespace sober {
namespace log {

class Logger {
 public:
  using Instance = boost::log::sources::logger;

  Logger(const char* name, const void* parent);

  static void init_logs_for_testing(bool log_to_cout, bool log_cout_verbose);

  Instance debug;
  Instance info;
  Instance error;

 private:
  void init(Instance&, const char* name, Severity severity, const void* parent);
};

} // namespace log
} // namespace sober

#endif // SOBER_LOG_LOGGER_HPP_
