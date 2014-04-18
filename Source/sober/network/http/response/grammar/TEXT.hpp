#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_TEXT_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_TEXT_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/grammar/CTL.hpp>
#include <sober/network/http/response/grammar/LWS.hpp>
#include <sober/network/http/response/attribute/TEXT.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// 2.2 Basic Rules
template <class Iterator>
struct TEXT: qi::grammar<Iterator, attribute::TEXT()> {
  using Base = qi::grammar<Iterator, attribute::TEXT()>;

  TEXT(): Base(text) {
    text %= *((qi::char_|lws) - ctl);
  }

  CTL<Iterator> ctl;
  LWS<Iterator> lws;
  qi::rule<Iterator, attribute::TEXT()> text;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_TEXT_HPP_
