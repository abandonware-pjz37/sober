# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

if(DEFINED SOBER_SUGAR_CMAKE_)
  return()
else()
  set(SOBER_SUGAR_CMAKE_ 1)
endif()

include(sugar_files)
include(sugar_include)

sugar_include(network)

sugar_files(SUGAR_SOURCES Exit.hpp)
