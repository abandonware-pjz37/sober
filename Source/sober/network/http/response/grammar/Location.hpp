#ifndef SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_LOCATION_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_LOCATION_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/grammar/CR.hpp>
#include <sober/network/http/response/attribute/Location.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace grammar {

// http://tools.ietf.org/html/rfc7231#section-7.1.2
template <class Iterator>
struct Location: qi::grammar<Iterator, attribute::Location()> {
  using Base = qi::grammar<Iterator, response::attribute::Location()>;

  Location(): Base(location) {
    str_location %= qi::lit("Location: ") >> *(qi::char_ - cr);
    location %= str_location;
  }

  CR<Iterator> cr;

  qi::rule<Iterator, std::string()> str_location;
  qi::rule<Iterator, response::attribute::Location()> location;
};

} // namespace grammar
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_GRAMMAR_LOCATION_HPP_
