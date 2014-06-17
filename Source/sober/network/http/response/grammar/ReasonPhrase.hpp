#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_REASONPHRASE_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_REASONPHRASE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/attribute/ReasonPhrase.hpp>
#include <sober/network/http/response/grammar/HTAB.hpp>
#include <sober/network/http/response/grammar/ObsText.hpp>
#include <sober/network/http/response/grammar/SP.hpp>
#include <sober/network/http/response/grammar/VCHAR.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// rfc7230, 3.1.2. Status Line
template <class Iterator>
struct ReasonPhrase: qi::grammar<Iterator, attribute::ReasonPhrase()> {
  using Base = qi::grammar<Iterator, response::attribute::ReasonPhrase()>;

  ReasonPhrase(): Base(reason_phrase) {
    reason_phrase %= *(htab | sp | vchar | obs_text);
  }

  HTAB<Iterator> htab;
  SP<Iterator> sp;
  VCHAR<Iterator> vchar;
  ObsText<Iterator> obs_text;

  qi::rule<Iterator, response::attribute::ReasonPhrase()> reason_phrase;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_REASONPHRASE_HPP_
