#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CHUNKSIZE_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CHUNKSIZE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#undef nil // workaround for Obj-C++ (need for fusion::nil)

#include <leathers/push>
#include <leathers/all>
# include <boost/spirit/home/qi.hpp>
#include <leathers/pop>

#include <sober/network/http/response/attribute/ChunkSize.hpp>

#include <sober/network/http/response/attribute/TransferEncoding.hpp>
#include <sober/network/http/response/grammar/CRLF.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// rfc7230, 4.1. Chunked Transfer Coding
// Note: originally chunk-size do not include CRLF
template <class Iterator>
struct ChunkSize: qi::grammar<Iterator, attribute::ChunkSize()> {
  using Base = qi::grammar<Iterator, response::attribute::ChunkSize()>;

  ChunkSize(): Base(chunk_size) {
    chunk_size %= (qi::hex >> crlf);
  }

  CRLF<Iterator> crlf;
  qi::rule<Iterator, response::attribute::ChunkSize()> chunk_size;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CHUNKSIZE_HPP_
