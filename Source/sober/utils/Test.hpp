#ifndef SOBER_UTILS_TEST_HPP_
#define SOBER_UTILS_TEST_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <leathers/push>
#include <leathers/all>
# include <gtest/gtest.h> // ::testing::Test
#include <leathers/pop>

namespace sober {
namespace utils {

class Test: public ::testing::Test {
 public:
  using Base = ::testing::Test;

  static void SetUpTestCase();

  virtual ~Test();
};

} // namespace utils
} // namespace sober

#endif // SOBER_UTILS_TEST_HPP_
