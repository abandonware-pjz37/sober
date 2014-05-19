// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/Response.hpp>

#include <boost/log/sources/record_ostream.hpp> // BOOST_LOG
#include <boost/spirit/home/qi/action/action.hpp> // spirit::qi::parse
#include <sober/log/Severity.hpp>

namespace sober {
namespace network {
namespace http {

Response::Response(std::size_t buffer_size, Sink& sink):
    sink_(sink),
    log_info_(*this, log::Severity::INFO),
    log_debug_(*this, log::Severity::DEBUG),
    buffer_(std::max(buffer_size, MIN_BUFFER_SIZE)) {
}

bool Response::on_read() noexcept {
  using Encoding = response::attribute::TransferEncoding;
  using qi = boost::spirit::qi;

  assert(buffer_.size() > 0);

  if (!header_parsed_) {
    Iterator begin = buffer_.begin();
    Iterator end = buffer_.end();

    header_parsed_ = qi::parse(begin, end, header_grammar, header_);
    const char* message =
        header_parsed_ ? "Header parse failed" : "Successfully parsed";
    BOOST_LOG(log_debug_) << message << std::endl <<
        std::string(buffer_.begin(), buffer_.end());
    if (!header_parsed_) {
      return false;
    }

    buffer_.erase(buffer_.begin(), begin);

    // Note: if encoding is chunked, content_length is zero
    bytes_left_ = header_.content_length;
  }

  if (header_.transfer_encoding == Encoding::OTHER) {
    // If message body is not chunked then it has a fixed length.
    // bytes_left_ is number of bytes left to read.
    // Send all buffer to user and reduce bytes_left_ correspondingly.
    const std::size_t avail = std::min(bytes_left_, buffer_.size());
    bytes_left_ -= avail;
    const bool finish = (bytes_left_ == 0);
    sink.write(buffer_.data(), avail, finish);
    buffer_.erase(buffer_.begin(), buffer_.begin() + avail);
    return !finish;
  }

  assert(header_.transfer_encoding == Encoding::CHUNKED);
  while (true) {
    if (!chunk_size_done_) {
      Iterator begin = buffer_.begin();
      Iterator end = buffer_.end();
      const bool ok = qi::parse(begin, end, chunk_size_grammar, chunk_size_);
      if (!ok) {
        // need more data
        return false;
      }
      buffer_.erase(buffer.begin(), begin);
      chunk_size_done_ = true;
      bytes_left_ = chunk_size_;
    }

    assert(chunk_size_done_);
    if (bytes_left_ == 0) {
      // No more data left in chunk, expected crlf
      Iterator begin = buffer_.begin();
      Iterator end = buffer_.end();
      const bool ok = qi::parse(begin, end, crlf);
      if (!ok) {
        // need more data
        return false;
      }
      buffer_.erase(buffer.begin(), begin);
      if (chunk_size_ == 0) {
        // This is the last chunk
        sink.write(nullptr, 0, true);
        return true;
      }
      chunk_size_done_ = false;
      continue;
    }

    assert(chunk_size_done_);
    assert(bytes_left_ > 0);
    const std::size_t avail = std::min(bytes_left_, buffer_.size());
    if (avail == 0) {
      return false;
    }
    bytes_left_ -= avail;
    sink.write(buffer_.data(), avail, false);
    buffer_.erase(buffer_.begin(), buffer_.begin() + avail);
  }
}

void Response::clear() noexcept {
  sink_.clear();
  header_parsed_ = false;
  buffer_.clear();
}

const char* Response::log_name() const {
  return "sober.network.http.Response";
}

} // namespace http
} // namespace network
} // namespace sober
