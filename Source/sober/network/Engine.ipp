#ifndef SOBER_NETWORK_ENGINE_IPP_
#define SOBER_NETWORK_ENGINE_IPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/Engine.hpp>

namespace sober {
namespace network {

template <class Handler>
void Engine::async_resolve(
    const std::string& host,
    const std::string& port,
    Handler&& handler
) {
  resolver_.async_resolve(
      Resolver::query(host, port),
      handler // TODO std::forward not working
  );
}

} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_ENGINE_IPP_
