#ifndef SOBER_EXIT_HPP_
#define SOBER_EXIT_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <cstdlib> // EXIT_SUCCESS

namespace sober {

class Exit {
 public:
  enum {
#if defined(__APPLE__) && defined(__MACH__) && \
    defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
    // Special iOS case (http://stackoverflow.com/questions/21605916)
    // TODO: boost.predef BOOST_OS_IOS
    OK = 1,
#else
    OK = EXIT_SUCCESS,
#endif
    FAIL = EXIT_FAILURE
  };
};

} // namespace sober

#endif // SOBER_EXIT_HPP_
