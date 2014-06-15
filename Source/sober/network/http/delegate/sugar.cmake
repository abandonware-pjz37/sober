# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

if(DEFINED SOBER_NETWORK_HTTP_DELEGATE_SUGAR_CMAKE_)
  return()
else()
  set(SOBER_NETWORK_HTTP_DELEGATE_SUGAR_CMAKE_ 1)
endif()

include(sugar_files)

sugar_files(
    SOBER_SOURCES
    Interface.cpp
    Interface.fpp
    Interface.hpp
    Json.cpp
    Json.hpp
    Reconnect.cpp
    Reconnect.hpp
    Retry.cpp
    Retry.hpp
    String.cpp
    String.hpp
    Timeout.cpp
    Timeout.hpp
)
