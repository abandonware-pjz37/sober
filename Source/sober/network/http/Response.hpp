#ifndef SOBER_NETWORK_HTTP_RESPONSE_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <sober/log/Logger.hpp>
#include <sober/network/http/Stream.fpp>

namespace sober {
namespace network {
namespace http {

class Response {
 public:
  using Socket = boost::asio::ip::tcp::socket;
  using Error = boost::system::error_code;

  Response(const Stream& stream);
  Response(const Stream& stream, Response&&);

  Response(Response&&) = delete;
  Response& operator=(Response&&) = delete;

  Response(const Response&) = delete;
  Response& operator=(const Response&) = delete;

  template <class Handler>
  void async_read(
      Socket& socket,
      Handler&& handler
  );

  bool is_valid() const;
  const char* log_name() const;

  /**
    * @throw @c std::runtime_error if is_valid is false
    */
  const std::string& get_body() const;

  void clear();

 private:
  class CompletionCondition {
   public:
    CompletionCondition(Response& response): response_(response) {
    }

    std::size_t operator()(const Error& error, std::size_t bytes_transferred) {
      return response_.completion_condition(error, bytes_transferred);
    }

   private:
    Response& response_;
  };

  std::size_t completion_condition(
      const Error& error, std::size_t bytes_transferred
  );

  const Stream& stream_;
  log::Logger log_info_;
  log::Logger log_debug_;
  boost::asio::streambuf streambuf_;
  std::string body_;

  /**
    * @brief body_ contains successful HTTP response message body
    */
  bool is_valid_;

  CompletionCondition completion_condition_;
};

} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_HPP_
