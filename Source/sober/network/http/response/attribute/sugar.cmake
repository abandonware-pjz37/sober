# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

if(DEFINED SOBER_NETWORK_HTTP_RESPONSE_ATTRIBUTE_SUGAR_CMAKE_)
  return()
else()
  set(SOBER_NETWORK_HTTP_RESPONSE_ATTRIBUTE_SUGAR_CMAKE_ 1)
endif()

include(sugar_files)

sugar_files(
    SOBER_SOURCES
    ChunkSize.hpp
    ContentLength.hpp
    HTAB.hpp
    Location.hpp
    ObsText.hpp
    ReasonPhrase.hpp
    SP.hpp
    StatusCode.cpp
    StatusCode.fpp
    StatusCode.hpp
    StatusLine.hpp
    TransferEncoding.hpp
    VCHAR.hpp
)
