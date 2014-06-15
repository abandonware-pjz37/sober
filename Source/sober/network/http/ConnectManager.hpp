#ifndef SOBER_NETWORK_HTTP_CONNECT_MANAGER_HPP_
#define SOBER_NETWORK_HTTP_CONNECT_MANAGER_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <leathers/push>
#include <leathers/all>
# include <boost/asio/ip/tcp.hpp>
#include <leathers/pop>

#include <sober/log/Logger.hpp>
#include <sober/network/Engine.fpp>

namespace network {
class uri;
} // namespace network

namespace sober {
namespace network {
namespace http {

/**
  * @brief Cache resolved and connected endpoint
  */
class ConnectManager {
 public:
  using Iterator = boost::asio::ip::tcp::resolver::iterator;
  using Error = boost::system::error_code;
  using Socket = boost::asio::ip::tcp::socket;

  ConnectManager(Engine&);

  /**
    * @throws @c std::runtime_error if @param uri is incorrect
    */
  void set_endpoint(const ::network::uri& uri);

  template <class Handler> void resolve(Handler handler);
  template <class Handler> void connect(
      Handler handler, Socket& socket, Iterator
  );

  void clear_resolved();
  void clear_connected();

  void on_successful_connect(Iterator iterator);

  const std::string& host() const;

  bool is_resolved() const;
  bool is_connected() const;

 private:
  static std::string get_port(const ::network::uri& uri);

  Engine& engine_;

  log::Logger log_;

  std::string host_;
  std::string port_;

  Iterator resolver_iterator_;
  Iterator connected_iterator_;
};

} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_CONNECT_MANAGER_HPP_
