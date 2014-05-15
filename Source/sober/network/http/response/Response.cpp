// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/Response.hpp>

#include <boost/log/sources/record_ostream.hpp> // BOOST_LOG
#include <sober/log/Severity.hpp>
#include <sober/network/http/response/Parser.ipp>

namespace sober {
namespace network {
namespace http {
namespace response {

Response::Response(const Stream& stream):
    stream_(stream),
    log_info_(*this, log::Severity::INFO),
    log_debug_(*this, log::Severity::DEBUG),
    is_valid_(false),
    completion_condition_(*this) {
}

Response::Response(const Stream& stream, Response&& r):
    stream_(stream),
    log_info_(*this, log::Severity::INFO),
    log_debug_(*this, log::Severity::DEBUG),
    streambuf_(), // must be empty
    body_(std::move(r.body_)),
    is_valid_(r.is_valid_),
    completion_condition_(*this) {
  if (r.streambuf_.size() != 0) {
    throw std::runtime_error("Moving not empty Response");
  }
}

bool Response::is_valid() const {
  return is_valid_;
}

const char* Response::log_name() const {
  return "sober.network.http.Response";
}

const std::string& Response::get_body() const {
  if (!is_valid()) {
    throw std::runtime_error("Response body is not valid");
  }
  return body_;
}

void Response::clear() {
  is_valid_ = false;
  streambuf_.consume(streambuf_.size());
}

std::size_t Response::completion_condition(
    const Error &error, std::size_t bytes_transferred
) {
  const std::size_t stop_reading = 0;

  // TODO: Size is always 512 for streambuf
  const std::size_t continue_reading = std::numeric_limits<std::size_t>::max();
  assert(continue_reading != stop_reading);

  if (error) {
    assert(!is_valid());
    return stop_reading;
  }

  if (bytes_transferred == 0) {
    BOOST_LOG(log_info_) <<
        "read not done yet (" << bytes_transferred << " bytes)";
    return continue_reading;
  }

  boost::asio::const_buffers_1 buffers = streambuf_.data();
  const char* begin = boost::asio::buffer_cast<const char*>(buffers);
  const std::size_t size = boost::asio::buffer_size(buffers);

  if (size != bytes_transferred) {
    throw std::runtime_error("Internal error size/bytes_transferred");
  }

  try {
    const bool ok = Parser::parse(begin, begin + size, body_);
    if (ok) {
      is_valid_ = true;
      assert(is_valid()); // sanity check
      return stop_reading;
    }
    BOOST_LOG(log_info_) <<
        "read not done yet (" << bytes_transferred << " bytes)";
    BOOST_LOG(log_debug_) << std::endl << std::string(begin, begin + size);
    return continue_reading;
  }
  catch (std::exception& exc) {
    BOOST_LOG(log_info_) << "parse failed with exception: " << exc.what();
    BOOST_LOG(log_debug_) << "Failed with data: " << std::endl
        << std::string(begin, begin + size);
    assert(!is_valid());
    return stop_reading;
  }
}

} // namespace response
} // namespace http
} // namespace network
} // namespace sober
