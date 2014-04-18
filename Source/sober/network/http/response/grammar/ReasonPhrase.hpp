#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_REASONPHRASE_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_REASONPHRASE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/grammar/TEXT.hpp>
#include <sober/network/http/response/grammar/CR.hpp>
#include <sober/network/http/response/grammar/LF.hpp>
#include <sober/network/http/response/attribute/ReasonPhrase.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// 6.1.1 Status Code and Reason Phrase
template <class Iterator>
struct ReasonPhrase: qi::grammar<Iterator, attribute::ReasonPhrase()> {
  using Base = qi::grammar<Iterator, attribute::ReasonPhrase()>;

  ReasonPhrase(): Base(reason_phrase) {
    reason_phrase %= *(text - cr - lf);
  }

  TEXT<Iterator> text;
  CR<Iterator> cr;
  LF<Iterator> lf;

  qi::rule<Iterator, attribute::ReasonPhrase()> reason_phrase;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_REASONPHRASE_HPP_
