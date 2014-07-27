#ifndef SOBER_NETWORK_HTTP_EXTRA_SUCCESS_HANDLER_HPP_
#define SOBER_NETWORK_HTTP_EXTRA_SUCCESS_HANDLER_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <functional> // std::function

namespace sober {
namespace network {
namespace http {

using ExtraSuccessHandler = std::function<void()>;

} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_EXTRA_SUCCESS_HANDLER_HPP_
