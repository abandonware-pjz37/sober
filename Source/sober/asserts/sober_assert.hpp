#ifndef SOBER_ASSERTS_SOBER_ASSERT_HPP_
#define SOBER_ASSERTS_SOBER_ASSERT_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <leathers/push>
#include <leathers/all>
# include <boost/concept_check.hpp> // ignore_unused_variable_warning
#include <leathers/pop>

#include <sober/config.hpp> // SOBER_DEBUG
#include <sober/utils/always.hpp>

#if (SOBER_DEBUG)
# define SOBER_ASSERT_TRUE(expression) \
    do { BOOST_ASSERT(expression); } while (!sober::utils::always())
# define SOBER_ASSERT_TRUE_VAR(variable) \
    SOBER_ASSERT_TRUE(variable)
#else
# define SOBER_ASSERT_TRUE(expression) \
    do {} while (!sober::utils::always())
# define SOBER_ASSERT_TRUE_VAR(variable) \
    do { \
        ::boost::ignore_unused_variable_warning(variable); \
    } while (!sober::utils::always())
#endif

#endif // SOBER_ASSERTS_SOBER_ASSERT_HPP_
