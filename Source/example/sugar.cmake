# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

if(DEFINED SOBER_EXAMPLE_SUGAR_CMAKE_)
  return()
else()
  set(SOBER_EXAMPLE_SUGAR_CMAKE_ 1)
endif()

include(sugar_files)

sugar_files(
    SOBER_EXAMPLE_SOURCES
    empty.cpp
    openweathermap.cpp
)
