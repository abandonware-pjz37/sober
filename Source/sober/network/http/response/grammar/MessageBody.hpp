#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_MESSAGE_BODY_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_MESSAGE_BODY_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <boost/spirit/home/qi.hpp>

#include <sober/network/http/response/attribute/ContentLength.hpp>
#include <sober/network/http/response/attribute/TransferEncoding.hpp>
#include <sober/network/http/response/grammar/CRLF.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

template <class Iterator>
struct MessageBody: qi::grammar<
    Iterator, std::string(attribute::ContentLength, attribute::TransferEncoding)
> {
  using Base = qi::grammar<
      Iterator,
      std::string(attribute::ContentLength, attribute::TransferEncoding)
  >;

  MessageBody(): Base(message_body) {
    using Encoding = attribute::TransferEncoding;

    length_body %= qi::repeat(qi::_r1)[qi::char_];

    chunked_end = qi::lit("0") >> crlf >> crlf;

    chunk %= qi::omit[qi::hex[qi::_a = qi::_1]] >> crlf >>
        qi::repeat(qi::_a)[qi::char_] >> crlf;

    chunked_part %= chunk - chunked_end;

    chunked_body = qi::eps[qi::_val = ""] >>
        *chunked_part[qi::_val += qi::_1] >>
        chunked_end;

    message_body %= (qi::eps(qi::_r1 > 0) >> length_body(qi::_r1)) |
        (qi::eps(qi::_r2 == Encoding::CHUNKED) >> chunked_body);
  }

  CRLF<Iterator> crlf;

  qi::rule<
      Iterator,
      std::string(attribute::ContentLength, attribute::TransferEncoding)
  > message_body;

  qi::rule<Iterator, std::string(attribute::ContentLength)> length_body;
  qi::rule<Iterator, void()> chunked_end;
  qi::rule<Iterator, std::string(), qi::locals<int> > chunk;
  qi::rule<Iterator, std::string()> chunked_part;
  qi::rule<Iterator, std::string()> chunked_body;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_MESSAGE_BODY_HPP_
