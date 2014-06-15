#ifndef SOBER_NETWORK_HTTP_RESPONSE_IPP_
#define SOBER_NETWORK_HTTP_RESPONSE_IPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/Response.hpp>

#include <boost/spirit/home/qi/action/action.hpp> // spirit::qi::parse

namespace sober {
namespace network {
namespace http {

template <class Handler>
inline void Response::async_read_some(
    Socket& socket,
    Handler&& handler
) BOOST_NOEXCEPT {
  socket.async_read_some(buffer_.prepare(buffer_size_), handler);
}

template <class Grammar, class Attribute>
std::size_t Response::parse(const Grammar& grammar, Attribute& attribute) {
  namespace qi = boost::spirit::qi;

  Iterator begin = data_ptr();

  const bool ok = qi::parse(begin, begin + buffer_.size(), grammar, attribute);
  if (!ok) {
    return 0;
  }

  // TODO (check conversion)
  const std::size_t length = static_cast<std::size_t>(begin - data_ptr());

  assert(length != 0);
  buffer_.consume(length);
  return length;
}

} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_IPP_
