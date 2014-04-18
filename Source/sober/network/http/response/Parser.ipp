#ifndef SOBER_NETWORK_HTTP_RESPONSE_PARSER_IPP_
#define SOBER_NETWORK_HTTP_RESPONSE_PARSER_IPP_

#include <sober/network/http/response/Parser.hpp>
#include <sober/network/http/response/grammar/Response.hpp>
#include <sober/network/http/response/attribute/Response.hpp>

#include <boost/spirit/home/qi/action/action.hpp>

namespace sober {
namespace network {
namespace http {
namespace response {

template <class Iterator>
bool Parser::parse(Iterator begin, Iterator end, std::string& body) {
  grammar::Response<Iterator> grammar;
  attribute::Response attr;

  const bool result = boost::spirit::qi::parse(begin, end, grammar, attr);
  if (!result) {
    return false;
  }

  if (attr.status_line.status_code != attribute::StatusCode::OK) {
    throw std::runtime_error("Response Status-Code is not OK");
  }

  body = attr.message_body;
  return true;
}

} // namespace response
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_PARSER_IPP_
