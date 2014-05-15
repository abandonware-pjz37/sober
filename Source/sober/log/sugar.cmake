# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

if(DEFINED SOBER_LOG_SUGAR_CMAKE_)
  return()
else()
  set(SOBER_LOG_SUGAR_CMAKE_ 1)
endif()

include(sugar_files)

sugar_files(
    SOBER_SOURCES
    Logger.cpp
    Logger.hpp
    Severity.cpp
    Severity.fpp
    Severity.hpp
    attributes.cpp
    attributes.hpp
)
