// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/ConnectManager.hpp>

#include <leathers/push>
#include <leathers/all>
# include <boost/log/attributes/constant.hpp>
# include <boost/log/sources/record_ostream.hpp> // BOOST_LOG
# include <network/uri/uri.hpp>
#include <leathers/pop>

#include <sober/log/Severity.hpp>
#include <sober/network/http/Request.hpp>

namespace sober {
namespace network {
namespace http {

ConnectManager::ConnectManager(Engine& engine):
    engine_(engine),
    log_("sober.network.http.ConnectManager", this) {
}

void ConnectManager::set_endpoint(const ::network::uri& uri) {
  if (!uri.host().is_initialized()) {
    throw std::runtime_error("URI host is empty");
  }
  const std::string new_host = uri.host()->to_string();
  const std::string new_port = get_port(uri);

  if (host_ != new_host || port_ != new_port) {
    host_ = new_host;
    port_ = new_port;

    resolver_iterator_ = Iterator();
    connected_iterator_ = Iterator();

    BOOST_LOG(log_.info) << "host:" << host_ << ", port:" << port_;
  }

  BOOST_LOG(log_.debug) << "host:" << host_ << ", port:" << port_;

  if (host_.empty()) {
    throw std::runtime_error("Stream::set_endpoint host empty");
  }

  if (port_.empty()) {
    throw std::runtime_error("Stream::set_endpoint port empty");
  }
}

void ConnectManager::clear_resolved() {
  BOOST_LOG(log_.info) << "clear resolved";
  resolver_iterator_ = Iterator();
  connected_iterator_ = Iterator();
  assert(!is_resolved()); // sanity check
}

void ConnectManager::clear_connected() {
  BOOST_LOG(log_.info) << "clear connected";
  connected_iterator_ = Iterator();
  assert(!is_connected()); // sanity check
}

void ConnectManager::on_successful_connect(Iterator iterator) {
  if (connected_iterator_ == iterator) {
    return;
  }
  connected_iterator_ = iterator;
  BOOST_LOG(log_.info) << "connected to: " << connected_iterator_->endpoint();
}

const std::string& ConnectManager::host() const {
  if (host_.empty()) {
    throw std::runtime_error("host is empty");
  }
  return host_;
}

bool ConnectManager::is_connected() const {
  return connected_iterator_ != Iterator();
}

bool ConnectManager::is_resolved() const {
  return resolver_iterator_ != Iterator();
}

std::string ConnectManager::get_port(const ::network::uri &uri) {
  if (uri.port()) {
    return uri.port()->to_string();
  }

  if (*uri.scheme() == "http") {
    return "80";
  }

  if (*uri.scheme() == "https") {
    return "443";
  }

  throw std::runtime_error("Can't get port");
}

} // namespace http
} // namespace network
} // namespace sober
