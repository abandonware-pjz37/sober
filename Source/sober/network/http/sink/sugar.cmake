# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

if(DEFINED SOBER_NETWORK_HTTP_SINK_SUGAR_CMAKE_)
  return()
else()
  set(SOBER_NETWORK_HTTP_SINK_SUGAR_CMAKE_ 1)
endif()

include(sugar_files)

sugar_files(
    SOBER_SOURCES
    String.cpp
    String.hpp
)
