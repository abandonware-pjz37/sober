# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

if(DEFINED SOBER_UTILS_SUGAR_CMAKE_)
  return()
else()
  set(SOBER_UTILS_SUGAR_CMAKE_ 1)
endif()

include(sugar_files)

sugar_files(
    SOBER_SOURCES
    Test.cpp
    Test.hpp
    always.hpp
    run_duration.hpp
)
