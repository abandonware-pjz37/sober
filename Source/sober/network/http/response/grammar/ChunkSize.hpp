#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CHUNKSIZE_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CHUNKSIZE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <boost/spirit/home/qi.hpp>

#include <sober/network/http/response/attribute/ChunkSize.hpp>

#include <sober/network/http/response/attribute/TransferEncoding.hpp>
#include <sober/network/http/response/grammar/CRLF.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

template <class Iterator>
struct ChunkSize: qi::grammar<Iterator, attribute::ChunkSize()> {
  using Base = qi::grammar<Iterator, attribute::ChunkSize()>;

  ChunkSize(): Base(chunk_size) {
    chunk_size %= (qi::hex >> crlf);
  }

  CRLF<Iterator> crlf;
  qi::rule<Iterator, attribute::ChunkSize()> chunk_size;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_CHUNKSIZE_HPP_