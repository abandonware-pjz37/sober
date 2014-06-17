#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_OBSTEXT_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_OBSTEXT_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/attribute/ObsText.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// rfc7230, 3.2.6. Field Value Components
template <class Iterator>
struct ObsText: qi::grammar<Iterator, attribute::ObsText()> {
  using Base = qi::grammar<Iterator, response::attribute::ObsText()>;

  ObsText(): Base(obs_text) {
    obs_text %= qi::ascii::char_(0x80, 0xff);
  }

  qi::rule<Iterator, response::attribute::ObsText()> obs_text;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_OBSTEXT_HPP_
