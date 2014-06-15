// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/utils/Test.hpp>

#include <leathers/push>
#include <leathers/all>
# include <boost/log/sinks.hpp>
# include <boost/log/utility/setup.hpp>
# include <boost/utility/empty_deleter.hpp>
# include <fstream> // std::ofstream
#include <leathers/pop>

#include <sober/log/attributes.hpp> // sober::log::formatter
#include <sober/log/Logger.hpp>

namespace sober {
namespace utils {

void Test::SetUpTestCase() {
  Base::SetUpTestCase();

  const bool log_to_cout = false;
  const bool log_cout_verbose = false;

  log::Logger::init_logs_for_testing(log_to_cout, log_cout_verbose);
}

Test::~Test() {
}

} // namespace utils
} // namespace sober
