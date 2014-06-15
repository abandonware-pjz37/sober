#ifndef SOBER_NETWORK_HTTP_RESPONSE_HPP_
#define SOBER_NETWORK_HTTP_RESPONSE_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <leathers/push>
#include <leathers/all>
# include <boost/asio/ip/tcp.hpp>
# include <boost/asio/streambuf.hpp>
#include <leathers/pop>

#include <sober/log/Logger.hpp>
#include <sober/network/http/Stream.fpp>
#include <sober/network/http/delegate/Interface.fpp>
#include <sober/network/http/response/grammar/ChunkSize.hpp>
#include <sober/network/http/response/grammar/Header.hpp>
#include <sober/network/http/unittest/Response.fpp>

namespace sober {
namespace network {
namespace http {

class Response {
 public:
  using Socket = boost::asio::ip::tcp::socket;
  using Error = boost::system::error_code;

  Response();

  void set_buffer_size(std::size_t buffer_size);

  void set_delegate(delegate::Interface&);

  template <class Handler>
  void async_read_some(Socket& socket, Handler&& handler) BOOST_NOEXCEPT;

  /**
    * @return true - Stop async reading (read successful)
    * @return false - Need more data, continue async reading
    * @todo Optimization: check parse error (for now try parse until EOF)
    */
  bool on_read(std::size_t bytes_transferred) BOOST_NOEXCEPT;

  /**
    * @brief Clear sink and buffer
    */
  void clear() BOOST_NOEXCEPT;

  const response::attribute::Header& header() const;

 private:
  friend class ::sober::network::http::unittest::Response;

  static const std::size_t DEFAULT_BUFFER_SIZE = 512;

  /**
    * @brief For testing only
    */
  Response(const std::string& input, delegate::Interface&);

  using Buffer = boost::asio::streambuf;
  using Iterator = const char*;

  Iterator data_ptr() const BOOST_NOEXCEPT;

  template <class Grammar, class Attribute>
  std::size_t parse(const Grammar& grammar, Attribute& attribute);

  delegate::Interface* delegate_;

  log::Logger log_;

  std::size_t buffer_size_;

  /**
    *
    */
  bool header_parsed_;
  response::attribute::Header header_;

  /**
    * @details rfc7230, 4.1 Chunked Transfer Coding:
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

  const response::grammar::Header<Iterator> header_grammar_;
  const response::grammar::ChunkSize<Iterator> chunk_size_grammar_;
  const response::grammar::CRLF<Iterator> crlf_grammar_;

  Buffer buffer_;
};

} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_RESPONSE_HPP_
