# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

if(DEFINED SOBER_NETWORK_HTTP_SUGAR_CMAKE_)
  return()
else()
  set(SOBER_NETWORK_HTTP_SUGAR_CMAKE_ 1)
endif()

include(sugar_files)
include(sugar_include)

sugar_include(delegate)
sugar_include(request)
sugar_include(response)
sugar_include(unittest)

sugar_files(
    SOBER_SOURCES
    ConnectManager.cpp
    ConnectManager.hpp
    ConnectManager.ipp
    ExtraSuccessHandler.hpp
    Request.cpp
    Request.fpp
    Request.hpp
    Request.ipp
    Response.cpp
    Response.hpp
    Response.ipp
    Statistic.cpp
    Statistic.hpp
    Stream.cpp
    Stream.fpp
    Stream.hpp
)
