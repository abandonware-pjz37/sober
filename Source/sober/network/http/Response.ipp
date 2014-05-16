#ifndef SOBER_NETWORK_HTTP_RESPONSE_IPP_
#define SOBER_NETWORK_HTTP_RESPONSE_IPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/Response.hpp>

namespace sober {
namespace network {
namespace http {

template <class Handler>
inline void Response::async_read(
    Socket& socket,
    Handler&& handler
) {
  clear();
  assert(!is_valid());
  assert(streambuf_.size() == 0);

  boost::asio::async_read(socket, streambuf_, completion_condition_, handler);
}

} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_IPP_
