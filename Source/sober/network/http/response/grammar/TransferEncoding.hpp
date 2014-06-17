#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_TRANSFERENCODING_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_TRANSFERENCODING_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/grammar/common.hpp> // qi
#include <sober/network/http/response/grammar/CR.hpp>
#include <sober/network/http/response/attribute/TransferEncoding.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// rfc7230, 3.3.1. Transfer-Encoding
template <class Iterator>
struct TransferEncoding: qi::grammar<Iterator, attribute::TransferEncoding()> {
  using Base = qi::grammar<Iterator, response::attribute::TransferEncoding()>;

  TransferEncoding(): Base(transfer_encoding) {
    using TE = response::attribute::TransferEncoding;

    // rfc7230, 4. Transfer Coding
    transfer_extension = *(qi::char_ - cr); // partial parse here, accept any
    transfer_coding %=
        qi::lit("chunked")[qi::_val = TE::CHUNKED] |
        transfer_extension[qi::_val = TE::OTHER];
    transfer_encoding %= "Transfer-Encoding: " >> transfer_coding;
  }

  CR<Iterator> cr;

  qi::rule<Iterator, void()> transfer_extension;
  qi::rule<Iterator, response::attribute::TransferEncoding()> transfer_coding;
  qi::rule<Iterator, response::attribute::TransferEncoding()> transfer_encoding;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_TRANSFERENCODING_HPP_
