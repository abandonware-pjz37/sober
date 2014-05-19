#ifndef SOBER_NETWORK_HTTP_RESPONSE_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <sober/log/Logger.hpp>
#include <sober/network/http/Stream.fpp>
#include <sober/network/http/response/grammar/Header.hpp>

namespace sober {
namespace network {
namespace http {

class Response {
 public:
  using Socket = boost::asio::ip::tcp::socket;
  using Error = boost::system::error_code;

  Response(std::size_t buffer_size, Sink& sink);

  template <class Handler>
  void async_read_some(Socket& socket, Handler&& handler) noexcept;

  /**
    * @return true - Stop async reading
    * @return false - Continue async reading
    */
  bool on_read() noexcept;

  /**
    * @brief Clear sink and buffer
    */
  void clear() noexcept;

  const char* log_name() const noexcept;

 private:
  using Buffer = std::vector<char>;
  using Iterator = Buffer::iterator;

  static const std::size_t MIN_BUFFER_SIZE = 512;

  Sink& sink;

  log::Logger log_info_;
  log::Logger log_debug_;

  /**
    *
    */
  bool header_parsed_;
  response::attribute::Header header_;

  /**
    * @detail rfc2616, 3.6.1 Chunked Transfer Coding:
    * chunk-size << crlf << chunk-data << crlf
    * where the last chunk has chunk-size = 0 and no chunk-data, i.e.:
    * "0" << crlf << crlf
    *
    * If chunk_size_done_ is false, then current read position is on chunk-size:
    *
    * chunk-size << crlf << chunk-data << crlf
    * ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    *
    * after size successfully read chunk_size_done_ set to true and
    * chunk_size_ variable init, current read position is on chunk-data:
    *
    * chunk-size << crlf << chunk-data << crlf
    *                       ^~~~~~~~~~~~~~~~~~
    * bytes_left_ variable init by chunk_size_ value and reduces while
    * reading data from chunk-data. When bytes_left_ is zero (all data
    * read) expected crlf
    */
  bool chunk_size_done_;
  std::size_t chunk_size_;
  std::size_t bytes_left_;

  const grammar::Header<Iterator> header_grammar_;
  const grammar::ChunkSize<Iterator> chunk_size_grammar_;

  Buffer buffer_;
};

} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_HPP_
