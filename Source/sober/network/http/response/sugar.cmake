# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

if(DEFINED SOBER_NETWORK_HTTP_RESPONSE_SUGAR_CMAKE_)
  return()
else()
  set(SOBER_NETWORK_HTTP_RESPONSE_SUGAR_CMAKE_ 1)
endif()

include(sugar_include)

sugar_include(attribute)
sugar_include(grammar)
