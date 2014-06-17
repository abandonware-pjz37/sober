#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_HEADER_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_HEADER_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <leathers/push>
#include <leathers/all>
# include <boost/phoenix.hpp>
# include <boost/spirit/home/qi.hpp>
# include <boost/spirit/home/qi/nonterminal/grammar.hpp>
# include <boost/spirit/home/support/common_terminals.hpp>
# include <boost/spirit/include/phoenix_fusion.hpp>
#include <leathers/pop>

#include <sober/network/http/response/attribute/Header.hpp>
#include <sober/network/http/response/grammar/CR.hpp>
#include <sober/network/http/response/grammar/CRLF.hpp>
#include <sober/network/http/response/grammar/ContentLength.hpp>
#include <sober/network/http/response/grammar/Location.hpp>
#include <sober/network/http/response/grammar/StatusLine.hpp>
#include <sober/network/http/response/grammar/TransferEncoding.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// rfc7230, 3. Message Format
// Header = HTTP-message without message-body
template <class Iterator>
struct Header: qi::grammar<Iterator, attribute::Header()> {
  using Base = qi::grammar<Iterator, response::attribute::Header()>;

  Header(): Base(header) {
    namespace ph = boost::phoenix;
    using qi::_val;
    using qi::_1;
    using ph::at_c;

    // Header:
    // at_c<0> StatusLine status_line;
    // at_c<1> ContentLength content_length;
    // at_c<2> TransferEncoding transfer_encoding;
    // at_c<3> Location location;

    any_header = +(qi::char_ - cr);

    // rfc7230, 3.1. Start Line
    // start-line = status-line (for response)
    header = qi::eps[at_c<1>(_val) = 0] >>
        qi::eps[at_c<2>(_val) = response::attribute::TransferEncoding::OTHER] >>
        status_line[at_c<0>(_val) = _1] >>
        // Headers (simplified version) --
        +(
            (
                content_length[at_c<1>(_val) = _1] |
                transfer_encoding[at_c<2>(_val) = _1] |
                location[at_c<3>(_val) = _1] |
                any_header
            ) >> crlf
        ) >>
        // --
        crlf;
    ;
  }

  StatusLine<Iterator> status_line;
  ContentLength<Iterator> content_length;
  TransferEncoding<Iterator> transfer_encoding;
  Location<Iterator> location;
  CR<Iterator> cr;
  CRLF<Iterator> crlf;

  qi::rule<Iterator, void()> any_header;

  qi::rule<Iterator, response::attribute::Header()> header;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_HEADER_HPP_
