// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/unittest/Response.fpp>

#include <leathers/push>
#include <leathers/all>
# include <gtest/gtest.h> // TEST_F
#include <leathers/pop>

#include <sober/network/http/Response.hpp>
#include <sober/network/http/delegate/String.hpp>
#include <sober/utils/Test.hpp>

#include <leathers/push>
#include <leathers/global-constructors>

namespace sober {
namespace network {
namespace http {
namespace unittest {

class Response : public utils::Test {
 public:
  using Pointer = std::unique_ptr<http::Response>;

  // Workaround for private constructor
  Pointer make_response(const std::string& message, delegate::Interface& d) {
    return Pointer(new http::Response(message, d));
  }

  virtual ~Response() override;
};

Response::~Response() {
}

TEST_F(Response, content_length_good) {
  std::string message(
      "HTTP/1.1 200 OK\r\n"
      "Date: Sun, 27 Apr 2014 19:02:53 GMT\r\n"
      "Server: Apache\r\n"
      "Content-Length: 12\r\n"
      "Content-Type: text/html;charset=ISO-8859-1\r\n"
      "\r\n"
      "Hello, body!"
  );

  delegate::String delegate;

  ASSERT_FALSE(delegate.ready());
  auto response = make_response(message, delegate);
  ASSERT_TRUE(delegate.ready());

  ASSERT_EQ(
      response->header().status_line.status_code,
      response::attribute::StatusCode::OK
  );
  ASSERT_STREQ("Hello, body!", delegate.body().c_str());
}

TEST_F(Response, content_length_bad) {
  std::string message(
      "HTTP/1.1 200 OK\r\n"
      "Date: Sun, 27 Apr 2014 19:02:53 GMT\r\n"
      "Server: Apache\r\n"
      "Content-Length: 13\r\n" // <--- bad size, must be 12
      "Content-Type: text/html;charset=ISO-8859-1\r\n"
      "\r\n"
      "Hello, body!"
  );

  delegate::String delegate;

  ASSERT_FALSE(delegate.ready());
  ASSERT_THROW(make_response(message, delegate), std::runtime_error);
  ASSERT_FALSE(delegate.ready());
}

TEST_F(Response, chunked_simple) {
  std::string message(
      "HTTP/1.1 200 OK\r\n"
      "Server: nginx\r\n"
      "Date: Sun, 27 Apr 2014 14:44:25 GMT\r\n"
      "Content-Type: application/json; charset=utf-8\r\n"
      "Transfer-Encoding: chunked\r\n"
      "Connection: keep-alive\r\n"
      "Access-Control-Allow-Origin: *\r\n"
      "Access-Control-Allow-Credentials: true\r\n"
      "Access-Control-Allow-Methods: GET, POST\r\n"
      "\r\n"
      "1\r\n"
      "A\r\n"
      "2\r\n"
      "BC\r\n"
      "0\r\n"
      "\r\n"
  );

  delegate::String delegate;

  ASSERT_FALSE(delegate.ready());
  auto response = make_response(message, delegate);
  ASSERT_TRUE(delegate.ready());

  ASSERT_STREQ("ABC", delegate.body().c_str());
  ASSERT_EQ(
      response->header().status_line.status_code,
      response::attribute::StatusCode::OK
  );
}

TEST_F(Response, chunked_forbidden) {
  std::string message(
      "HTTP/1.1 403 Forbidden\r\n"
      "Server: cloudflare-nginx\r\n"
      "Date: Thu, 15 May 2014 07:02:54 GMT\r\n"
      "Content-Type: text/html; charset=UTF-8\r\n"
      "Transfer-Encoding: chunked\r\n"
      "Connection: keep-alive\r\n"
      "Set-Cookie: __cfduid=; expires=Mon, 23-Dec-2019 23:50:00 GMT;"
          " path=/; HttpOnly\r\n"
      "Cache-Control: max-age=15\r\n"
      "Expires: Thu, 15 May 2014 07:03:09 GMT\r\n"
      "CF-RAY: -AMS\r\n"
      "\r\n"
      "0\r\n"
      "\r\n"
  );

  delegate::String delegate;

  ASSERT_FALSE(delegate.ready());
  auto response = make_response(message, delegate);
  ASSERT_TRUE(delegate.ready());
  ASSERT_STREQ("", delegate.body().c_str());

  ASSERT_EQ(
      response->header().status_line.status_code,
      response::attribute::StatusCode::FORBIDDEN
  );
}

TEST_F(Response, chunked_real) {
  std::string message(
      "HTTP/1.1 200 OK\r\n"
      "Server: nginx\r\n"
      "Date: Sun, 27 Apr 2014 14:44:25 GMT\r\n"
      "Content-Type: application/json; charset=utf-8\r\n"
      "Transfer-Encoding: chunked\r\n"
      "Connection: keep-alive\r\n"
      "Access-Control-Allow-Origin: *\r\n"
      "Access-Control-Allow-Credentials: true\r\n"
      "Access-Control-Allow-Methods: GET, POST\r\n"
      "\r\n"
      "1ad\r\n"
      "{\"coord\":{\"lon\":-95.56,\"lat\":33.66},\"sys\":{\"message\":"
      "0.0337,\"country\":\"US\",\"sunrise\":1398598610,\"sunset\":1398646960}"
      ",\"weather\":[{\"id\":800,\"main\":\"Clear\",\"description\":"
      "\"sky is clear\",\"icon\":\"01d\"}],\"base\":\"cmc stations\",\"main\""
      ":{\"temp\":296.15,\"pressure\":1001,\"humidity\":73,\"temp_min\":296.15"
      ",\"temp_max\":296.15},\"wind\":{\"speed\":5.7,\"deg\":170},\"rain\":"
      "{\"3h\":1},\"clouds\":{\"all\":1},\"dt\":1398605700,\"id\":4717560,"
      "\"name\":\"Paris\",\"cod\":200}\n\r\n"
      "0\r\n"
      "\r\n"
  );

  delegate::String delegate;

  ASSERT_FALSE(delegate.ready());
  auto response = make_response(message, delegate);
  ASSERT_TRUE(delegate.ready());
  ASSERT_EQ(
      response->header().status_line.status_code,
      response::attribute::StatusCode::OK
  );
}

TEST_F(Response, chunked_fail) {
  std::string message(
      "HTTP/1.1 200 OK\r\n"
      "Server: nginx\r\n"
      "Date: Wed, 21 May 2014 10:56:04 GMT\r\n"
      "Content-Type: application/json; charset=utf-8\r\n"
      "Transfer-Encoding: chunked\r\n"
      "Connection: keep-alive\r\n"
      "Access-Control-Allow-Origin: *\r\n"
      "Access-Control-Allow-Credentials: true\r\n"
      "Access-Control-Allow-Methods: GET, POST\r\n"
      "\r\n"
      "1e4\r\n"
      "{\"coord\":{\"lon\":-74.26,\"lat\":4.73},"
      "\"sys\":{\"message\":0.0092,\"country\":\"CO\","
      "\"sunrise\":1400668981,\"sunset\":1400713462},"
      "\"weather\":[{\"id\":804,\"main\":\"Clouds\","
      "\"description\":\"overcast clouds\",\"icon\":\"04n\"}],"
      "\"base\":\"cmc stations\",\"m"
  );

  delegate::String delegate;

  ASSERT_FALSE(delegate.ready());
  ASSERT_THROW(make_response(message, delegate), std::runtime_error);
  ASSERT_FALSE(delegate.ready());
}

TEST_F(Response, location) {
  std::string message(
      "HTTP/1.1 200 OK\r\n"
      "Location: http://some.location.com\r\n"
      "Content-Length: 12\r\n"
      "\r\n"
      "Hello, body!"
  );

  delegate::String delegate;

  ASSERT_FALSE(delegate.ready());
  const auto response = make_response(message, delegate);
  ASSERT_TRUE(delegate.ready());
  ASSERT_TRUE(response->header().location.is_initialized());
  ASSERT_STREQ(
      response->header().location.get().string().c_str(),
      "http://some.location.com"
  );
}

TEST_F(Response, no_location) {
  std::string message(
      "HTTP/1.1 200 OK\r\n"
      "Content-Length: 12\r\n"
      "\r\n"
      "Hello, body!"
  );

  delegate::String delegate;

  ASSERT_FALSE(delegate.ready());
  const auto response = make_response(message, delegate);
  ASSERT_TRUE(delegate.ready());
  ASSERT_FALSE(response->header().location.is_initialized());
}

} // namespace unittest
} // namespace http
} // namespace network
} // namespace sober

#include <leathers/pop>
