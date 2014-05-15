#ifndef SOBER_UTILS_TEST_HPP_
#define SOBER_UTILS_TEST_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <gtest/gtest.h> // ::testing::Test

namespace sober {
namespace utils {

class Test: public ::testing::Test {
 public:
  using Base = ::testing::Test;

  static void init_logs(bool log_to_cout, bool log_cout_verbose);

  static void SetUpTestCase();
};

} // namespace utils
} // namespace sober

#endif // SOBER_UTILS_TEST_HPP_
