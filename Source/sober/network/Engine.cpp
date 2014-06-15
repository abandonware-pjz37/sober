// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/Engine.hpp>

#include <leathers/push>
#include <leathers/all>
# include <boost/log/sources/record_ostream.hpp> // BOOST_LOG
#include <leathers/pop>

#include <sober/log/Severity.hpp>

namespace sober {
namespace network {

Engine::Engine():
    log_("sober.network.Engine", this),
    resolver_(io_service) {
}

void Engine::run() {
  BOOST_LOG(log_.debug) << "run";

  // If 'run' called twice 'reset' need to be called between
  io_service.reset();

  io_service.run();
}

} // namespace network
} // namespace sober
