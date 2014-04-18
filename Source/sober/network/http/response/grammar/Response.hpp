#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_RESPONSE_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_RESPONSE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <boost/spirit/home/qi.hpp>

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/support/common_terminals.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>

#include <sober/network/http/response/grammar/StatusLine.hpp>
#include <sober/network/http/response/grammar/ContentLength.hpp>
#include <sober/network/http/response/grammar/CR.hpp>
#include <sober/network/http/response/grammar/CRLF.hpp>
#include <sober/network/http/response/attribute/Response.hpp>

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

    any_header = +(qi::char_ - cr);
    not_content_length = *((any_header - content_length) >> crlf);

    response %= status_line >>
        // 7.1 Entity Header Fields (simplified version) --
        qi::omit[not_content_length] >>
        content_length >> crlf >>
        qi::omit[not_content_length] >>
        // --
        crlf >> qi::repeat(ph::at_c<1>(qi::_val))[qi::char_]
    ;
  }

  StatusLine<Iterator> status_line;
  ContentLength<Iterator> content_length;
  CR<Iterator> cr;
  CRLF<Iterator> crlf;

  qi::rule<Iterator, void()> any_header;
  qi::rule<Iterator, void()> not_content_length;

  qi::rule<Iterator, attribute::Response()> response;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_RESPONSE_HPP_
