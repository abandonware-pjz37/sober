#ifndef SOBER_NETWORK_ENGINE_HPP_
#define SOBER_NETWORK_ENGINE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/Engine.fpp>

#include <leathers/push>
#include <leathers/all>
# include <boost/asio/io_service.hpp>
# include <boost/asio/ip/tcp.hpp> // tcp::resolver
#include <leathers/pop>

#include <sober/log/Logger.hpp>

namespace sober {
namespace network {

class Engine {
 public:
  using Resolver = boost::asio::ip::tcp::resolver;

  Engine();

  template <class Handler>
  void async_resolve(
      const std::string& host,
      const std::string& port,
      Handler&& handler
  );

  void run();

  /**
    * @note io_service left as public for custom behaviour needs
    */
  boost::asio::io_service io_service;

 private:
  log::Logger log_;
  Resolver resolver_;
};

} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_ENGINE_HPP_
