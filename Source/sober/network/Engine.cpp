// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/Engine.hpp>

#include <boost/log/sources/record_ostream.hpp> // BOOST_LOG
#include <sober/log/Severity.hpp>

namespace sober {
namespace network {

Engine::Engine():
    log_debug_(*this, log::Severity::DEBUG),
    resolver_(io_service) {
}

void Engine::run() {
  BOOST_LOG(log_debug_) << "run";

  // If 'run' called twice 'reset' need to be called between
  io_service.reset();

  io_service.run();
}

const char* Engine::log_name() const {
  return "sober.network.Engine";
}

} // namespace network
} // namespace sober
