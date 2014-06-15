# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

if(DEFINED SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_SUGAR_CMAKE_)
  return()
else()
  set(SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_SUGAR_CMAKE_ 1)
endif()

include(sugar_files)

sugar_files(
    SOBER_SOURCES
    CR.hpp
    CRLF.hpp
    ChunkSize.hpp
    ContentLength.hpp
    HTAB.hpp
    HTTPVersion.hpp
    Header.hpp
    LF.hpp
    Location.hpp
    ObsText.hpp
    ReasonPhrase.hpp
    SP.hpp
    StatusCode.hpp
    StatusLine.hpp
    TransferEncoding.hpp
    VCHAR.hpp
    common.hpp
)
