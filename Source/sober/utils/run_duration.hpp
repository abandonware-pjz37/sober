#ifndef SOBER_UTILS_RUN_DURATION_HPP_
#define SOBER_UTILS_RUN_DURATION_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

namespace sober {
namespace utils {

template <class Clock, class R>
typename Clock::duration run_duration(R&& r) {
  typename Clock::time_point start = Clock::now();

  r();

  typename Clock::time_point end = Clock::now();

  return end - start;
}

} // namespace utils
} // namespace sober

#endif // SOBER_UTILS_RUN_DURATION_HPP_
