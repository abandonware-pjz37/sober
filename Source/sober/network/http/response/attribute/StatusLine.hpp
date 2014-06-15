#ifndef SOBER_NETWORK_HTTP_RESPONSE_ATTRIBUTE_STATUSLINE_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_ATTRIBUTE_STATUSLINE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <leathers/push>
#include <leathers/all>
# include <boost/fusion/adapted.hpp> // BOOST_FUSION_ADAPT_STRUCT
#include <leathers/pop>

#include <sober/network/http/response/attribute/StatusCode.hpp>
#include <sober/network/http/response/attribute/ReasonPhrase.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace attribute {

struct StatusLine {
  StatusCode status_code;
  ReasonPhrase reason_phrase;
};

} // namespace attribute
} // namespace response
} // namespace http
} // namespace network
} // namespace sober

BOOST_FUSION_ADAPT_STRUCT(
    sober::network::http::response::attribute::StatusLine,
    (sober::network::http::response::attribute::StatusCode, status_code)
    (sober::network::http::response::attribute::ReasonPhrase, reason_phrase)
)

#endif // SOBER_NETWORK_HTTP_RESPONSE_ATTRIBUTE_STATUSLINE_HPP_
