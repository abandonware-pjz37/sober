#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_RESPONSE_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_RESPONSE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <boost/spirit/home/qi.hpp>
#include <boost/phoenix.hpp>

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/support/common_terminals.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>

#include <sober/network/http/response/attribute/Response.hpp>
#include <sober/network/http/response/grammar/CR.hpp>
#include <sober/network/http/response/grammar/CRLF.hpp>
#include <sober/network/http/response/grammar/ContentLength.hpp>
#include <sober/network/http/response/grammar/MessageBody.hpp>
#include <sober/network/http/response/grammar/StatusLine.hpp>
#include <sober/network/http/response/grammar/TransferEncoding.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// 6 Response
template <class Iterator>
struct Response: qi::grammar<Iterator, attribute::Response()> {
  using Base = qi::grammar<Iterator, attribute::Response()>;

  Response(): Base(response) {
    namespace ph = boost::phoenix;
    using qi::_val;
    using qi::_1;
    using ph::at_c;

    // Response:
    // at_c<0> StatusLine status_line;
    // at_c<1> ContentLength content_length;
    // at_c<2> TransferEncoding transfer_encoding;
    // at_c<3> std::string message_body;

    any_header = +(qi::char_ - cr);

    response = qi::eps[at_c<1>(_val) = 0] >>
        qi::eps[at_c<2>(_val) = attribute::TransferEncoding::OTHER] >>
        status_line[at_c<0>(_val) = _1] >>
        // 7.1 Entity Header Fields (simplified version) --
        *(
            (
                content_length[at_c<1>(_val) = _1] |
                transfer_encoding[at_c<2>(_val) = _1] |
                any_header
            ) >> crlf
        ) >>
        // --
        crlf >> message_body(at_c<1>(_val), at_c<2>(_val))[at_c<3>(_val) = _1]
    ;
  }

  StatusLine<Iterator> status_line;
  ContentLength<Iterator> content_length;
  TransferEncoding<Iterator> transfer_encoding;
  CR<Iterator> cr;
  CRLF<Iterator> crlf;
  MessageBody<Iterator> message_body;

  qi::rule<Iterator, void()> any_header;

  qi::rule<Iterator, attribute::Response()> response;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_RESPONSE_HPP_
