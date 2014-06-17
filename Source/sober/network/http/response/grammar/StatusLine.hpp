#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_STATUSLINE_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_STATUSLINE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/grammar/common.hpp> // qi
#include <sober/network/http/response/grammar/HTTPVersion.hpp>
#include <sober/network/http/response/grammar/StatusCode.hpp>
#include <sober/network/http/response/grammar/ReasonPhrase.hpp>
#include <sober/network/http/response/grammar/SP.hpp>
#include <sober/network/http/response/grammar/CRLF.hpp>
#include <sober/network/http/response/attribute/StatusLine.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// rfc7230, 3.1.2. Status-Line
template <class Iterator>
struct StatusLine: qi::grammar<Iterator, attribute::StatusLine()> {
  using Base = qi::grammar<Iterator, response::attribute::StatusLine()>;

  StatusLine(): Base(status_line) {
    status_line %=
        http_version >> qi::omit[sp] >> status_code >> qi::omit[sp] >>
        reason_phrase >> crlf;
  }

  HTTPVersion<Iterator> http_version;
  StatusCode<Iterator> status_code;
  ReasonPhrase<Iterator> reason_phrase;
  SP<Iterator> sp;
  CRLF<Iterator> crlf;

  qi::rule<Iterator, response::attribute::StatusLine()> status_line;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_STATUSLINE_HPP_
