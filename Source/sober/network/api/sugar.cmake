# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

if(DEFINED SOBER_NETWORK_API_SUGAR_CMAKE_)
  return()
else()
  set(SOBER_NETWORK_API_SUGAR_CMAKE_ 1)
endif()

include(sugar_files)
include(sugar_include)

sugar_include(unittest)

sugar_files(
    SOBER_SOURCES
    OpenWeatherMap.cpp
    OpenWeatherMap.hpp
)
