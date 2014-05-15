# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

if(DEFINED SOBER_NETWORK_HTTP_RESPONSE_SUGAR_CMAKE_)
  return()
else()
  set(SOBER_NETWORK_HTTP_RESPONSE_SUGAR_CMAKE_ 1)
endif()

include(sugar_files)
include(sugar_include)

sugar_files(
    SOBER_SOURCES
    Parser.hpp
    Parser.ipp
    Response.cpp
    Response.hpp
    Response.ipp
)

sugar_include(attribute)
sugar_include(grammar)
sugar_include(unittest)
