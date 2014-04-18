#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_LWS_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_LWS_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/grammar/CRLF.hpp>
#include <sober/network/http/response/grammar/SP.hpp>
#include <sober/network/http/response/grammar/HT.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// 2.2 Basic Rules
template <class Iterator>
struct LWS: qi::grammar<Iterator, void()> {
  using Base = qi::grammar<Iterator, void()>;

  LWS(): Base(lws) {
    lws = (-crlf) >> +(sp | ht);
  }

  CRLF<Iterator> crlf;
  SP<Iterator> sp;
  HT<Iterator> ht;

  qi::rule<Iterator, void()> lws;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_LWS_HPP_
