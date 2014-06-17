// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/Response.hpp>

#include <leathers/push>
#include <leathers/all>
# include <algorithm> // std::max
# include <boost/log/sources/record_ostream.hpp> // BOOST_LOG
#include <leathers/pop>

#include <sober/log/Severity.hpp>
#include <sober/network/http/Response.ipp>
#include <sober/network/http/delegate/Interface.hpp>

namespace sober {
namespace network {
namespace http {

Response::Response():
    delegate_(nullptr),
    log_("sober.network.http.Response", this),
    buffer_size_(DEFAULT_BUFFER_SIZE) {
}

void Response::set_buffer_size(std::size_t buffer_size) {
  buffer_size_ = (buffer_size > 0) ? buffer_size : DEFAULT_BUFFER_SIZE;
}

void Response::set_delegate(delegate::Interface& delegate) {
  delegate_ = std::addressof(delegate);
}

bool Response::on_read(std::size_t bytes_transferred) BOOST_NOEXCEPT {
  using Encoding = response::attribute::TransferEncoding;

  buffer_.commit(bytes_transferred);

  if (buffer_.size() == 0) {
    BOOST_LOG(log_.debug) << "Need more data (buffer is empty)";
    return false;
  }

  assert(buffer_.size() > 0);

  if (!header_parsed_) {
    BOOST_LOG(log_.debug) << "Try parse:" << std::endl <<
        std::string(data_ptr(), data_ptr() + buffer_.size());

    const std::size_t len = parse(header_grammar_, header_);
    header_parsed_ = (len != 0);

    const char* message =
        header_parsed_ ?  "Header successfully parsed" : "Header parse failed";
    BOOST_LOG(log_.debug) << message;

    if (!header_parsed_) {
      // Need more data (TODO: or incorrect response?)
      return false;
    }

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
    if (delegate_ != nullptr) {
      delegate_->body_write(data_ptr(), avail);
      if (finish) {
        delegate_->body_finish();
      }
    }

    buffer_.consume(avail);
    return finish;
  }

  assert(header_.transfer_encoding == Encoding::CHUNKED);
  for (;;) {
    if (!chunk_size_done_) {
      const std::size_t len = parse(chunk_size_grammar_, chunk_size_);
      if (len == 0) {
        // Need more data (TODO: or incorrect response?)
        return false;
      }
      chunk_size_done_ = true;
      bytes_left_ = chunk_size_;
    }

    assert(chunk_size_done_);
    if (bytes_left_ == 0) {
      // No more data left in chunk, expected crlf
      boost::spirit::unused_type attribute;
      const std::size_t len = parse(crlf_grammar_, attribute);
      if (len == 0) {
        // Need more data (TODO: or incorrect response?)
        return false;
      }
      if (chunk_size_ == 0) {
        // This is the last chunk
        if (delegate_ != nullptr) {
          delegate_->body_finish();
        }
        return true;
      }
      chunk_size_done_ = false;
      continue;
    }

    assert(chunk_size_done_);
    assert(bytes_left_ > 0);
    const std::size_t avail = std::min(bytes_left_, buffer_.size());
    if (avail == 0) {
      // Need more data
      return false;
    }
    bytes_left_ -= avail;
    if (delegate_ != nullptr) {
      delegate_->body_write(data_ptr(), avail);
    }
    buffer_.consume(avail);
  }
}

void Response::clear() BOOST_NOEXCEPT {
  if (delegate_ != nullptr) {
    delegate_->body_start();
  }
  header_parsed_ = false;
  chunk_size_done_ = false;
  chunk_size_ = 0;
  bytes_left_ = 0;
  buffer_.consume(buffer_.size());
}

const response::attribute::Header& Response::header() const {
  if (!header_parsed_) {
    throw std::runtime_error("Header not parsed");
  }
  return header_;
}

Response::Iterator Response::data_ptr() const BOOST_NOEXCEPT {
  return boost::asio::buffer_cast<Response::Iterator>(buffer_.data());
}

Response::Response(const std::string& input, delegate::Interface& delegate):
    Response() {
  set_delegate(delegate);
  clear();

  std::ostream os(&buffer_);
  os << input;
  os.flush();

  assert(buffer_.size() == input.size());

  // Bytes already commited, so transfer zero bytes
  const bool finish = on_read(0);
  if (!finish) {
    throw std::runtime_error("Parse failed");
  }
}

} // namespace http
} // namespace network
} // namespace sober
