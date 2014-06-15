#ifndef SOBER_NETWORK_HTTP_REQUEST_HPP_
#define SOBER_NETWORK_HTTP_REQUEST_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/Request.fpp>

#include <leathers/push>
#include <leathers/all>
# include <boost/asio/ip/tcp.hpp> // socket
# include <boost/asio/streambuf.hpp>
#include <leathers/pop>

#include <sober/log/Logger.hpp>
#include <sober/network/http/request/Method.fpp>
#include <string>

namespace sober {
namespace network {
namespace http {

/**
  * @brief HTTP request class
  * @note Keep in mind: one @c http::Stream - one @c Request
  */
class Request {
 public:
  using Socket = boost::asio::ip::tcp::socket;

  Request();

  Request(Request&&) = delete;
  Request& operator=(Request&&) = delete;

  Request(const Request&) = delete;
  Request& operator=(const Request&) = delete;

  void set_method(request::Method method);

  void set_path(const char* path);

  /**
    * @brief Add key with value, like `("i", "abc")` and `("k", "xyz")`
    */
  void add_query(const char* key, const std::string& value);

  /**
    * @brief clear_query then add_query
    */
  void set_query(const char* key, const std::string& value);

  /**
    * @brief Set raw query string, like: `i=abc&k=xyz`
    */
  void set_query(const std::string& query);
  void clear_query();

  template <class Handler>
  void async_write(Socket& socket, const std::string& host, Handler&& handler);
  void verify_size_on_write_done(std::size_t bytes_transferred) const;

 private:
  void fill_streambuf(const std::string& host);

  log::Logger log_;

  boost::asio::streambuf request_;

  const char* method_;
  std::string path_;
  std::string query_;
};

} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_REQUEST_HPP_
