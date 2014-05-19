// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/Request.hpp>

#include <boost/log/sources/record_ostream.hpp> // BOOST_LOG
#include <network/uri/uri.hpp> // uri::encode_query
#include <sober/log/Severity.hpp>
#include <sober/network/http/Stream.hpp>
#include <sober/network/http/request/Method.hpp>

namespace sober {
namespace network {
namespace http {

Request::Request(const Stream& stream):
    stream_(stream),
    log_info_(*this, log::Severity::INFO),
    log_debug_(*this, log::Severity::DEBUG),
    method_(to_const_char(Method::GET)) {
}

Request::Request(const Stream& stream, Request&& r):
    stream_(stream),
    log_info_(*this, log::Severity::INFO),
    log_debug_(*this, log::Severity::DEBUG),
    request_(), // r.request_ must be empty
    method_(r.method_),
    path_(std::move(r.path_)),
    query_(std::move(r.query_)) {
  if (r.request_.size() != 0) {
    throw std::runtime_error("Moving not empty Request");
  }
}

void Request::set_method(Method method) {
  method_ = to_const_char(method);
  BOOST_LOG(log_info_) << "method: " << method_;
}

void Request::set_path(const char *path) {
  path_ = path;
  BOOST_LOG(log_info_) << "path: " << path_;
}

void Request::set_query(const char *key, const std::string& value) {
  query_ = "?";
  query_ += key;
  query_ += "=";

  ::network::uri::encode_query(
     value.begin(), value.end(), std::back_inserter(query_)
  );

  BOOST_LOG(log_info_) << "query: `" << query_
      << "` (key:" << key << ", value:" << value << ")";
}

void Request::set_query(const std::string& query) {
  query_ = "?";

  ::network::uri::encode_query(
     query.begin(), query.end(), std::back_inserter(query_)
  );

  BOOST_LOG(log_info_) << "query: " << query_;
}

void Request::clear_query() {
  query_.clear();
  BOOST_LOG(log_info_) << "clear query";
}

void Request::verify_size_on_write_done(std::size_t bytes_transferred) const {
  if (request_.size() != 0) {
    throw std::runtime_error("Send request is not empty, verify failed");
  }
  if (bytes_transferred == 0) {
    throw std::runtime_error("Zero bytes writen");
  }
}

const char* Request::log_name() const {
  return "sober.network.http.Request";
}

void Request::fill_streambuf() {
  BOOST_LOG(log_info_) << "host:" << stream_.host();

  request_.consume(request_.size()); // clear
  std::ostream os(&request_);

  if (path_.empty()) {
    throw std::runtime_error("Request::set_path not called");
  }

  os << method_ << " " << path_ << query_ << " HTTP/1.1\r\n";

  if (stream_.host().empty()) {
    throw std::runtime_error("Internal error: host is empty");
  }

  os << "Host: " << stream_.host() << "\r\n";
  os << "User-Agent: github.com/ruslo/sober\r\n";

  os << "\r\n";

  boost::asio::const_buffers_1 buffers = request_.data();
  const char* begin = boost::asio::buffer_cast<const char*>(buffers);
  const std::size_t size = boost::asio::buffer_size(buffers);

  BOOST_LOG(log_debug_) << std::endl << std::string(begin, begin + size);
}

} // namespace http
} // namespace network
} // namespace sober
