#ifndef SOBER_NETWORK_HTTP_CONNECT_MANAGER_IPP_
#define SOBER_NETWORK_HTTP_CONNECT_MANAGER_IPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/ConnectManager.hpp>

#include <leathers/push>
#include <leathers/all>
# include <boost/asio/connect.hpp> // asio::async_connect
# include <boost/log/sources/record_ostream.hpp> // BOOST_LOG
#include <leathers/pop>

namespace sober {
namespace network {
namespace http {

template <class Handler>
void ConnectManager::resolve(Handler handler) {
  if (resolver_iterator_ != Iterator()) {
    // If already resolved (iterator is valid), call handler immediatelly
    BOOST_LOG(log_.debug) << "already resolved";
    const Error error;
    handler(error, resolver_iterator_);
    return;
  }

  BOOST_LOG(log_.info) << "resolve host:" << host_ << ", port:" << port_;
  engine_.async_resolve(host(), port_, handler);
}

template <class Handler>
void ConnectManager::connect(
    Handler handler, Socket& socket, Iterator iterator
) {
  if (resolver_iterator_ != iterator) {
    resolver_iterator_ = iterator;
    connected_iterator_ = Iterator();

    BOOST_LOG(log_.info) << "try connect to: ";
    int i = 1;
    for (Iterator it = iterator; it != Iterator(); ++it, ++i) {
      BOOST_LOG(log_.info) << "  " << i << ": " << it->endpoint();
    }
  }

  if (connected_iterator_ != Iterator()) {
    // If already connected (iterator is valid), call handler immediatelly
    BOOST_LOG(log_.debug) << "already connected";
    const Error error;
    handler(error, connected_iterator_);
    return;
  }

  BOOST_LOG(log_.info) << "start connect";
  boost::asio::async_connect(socket, iterator, handler);
}

} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_CONNECT_MANAGER_IPP_
