// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <gtest/gtest.h> // TEST_F

#include <sober/network/http/response/Parser.ipp>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace unittest {

class Parser : public ::testing::Test {
};

TEST_F(Parser, content_length_good) {
  std::string message(
      "HTTP/1.1 200 OK\r\n"
      "Date: Sun, 27 Apr 2014 19:02:53 GMT\r\n"
      "Server: Apache\r\n"
      "Content-Length: 12\r\n"
      "Content-Type: text/html;charset=ISO-8859-1\r\n"
      "\r\n"
      "Hello, body!"
  );

  std::string body;
  const bool result = response::Parser::parse(
      message.begin(), message.end(), body
  );

  ASSERT_EQ(result, true);
  ASSERT_STREQ("Hello, body!", body.c_str());
}

TEST_F(Parser, content_length_bad) {
  std::string message(
      "HTTP/1.1 200 OK\r\n"
      "Date: Sun, 27 Apr 2014 19:02:53 GMT\r\n"
      "Server: Apache\r\n"
      "Content-Length: 13\r\n" // <--- bad size, must be 12
      "Content-Type: text/html;charset=ISO-8859-1\r\n"
      "\r\n"
      "Hello, body!"
  );

  std::string body;
  const bool result = response::Parser::parse(
      message.begin(), message.end(), body
  );

  ASSERT_EQ(result, false);
}

TEST_F(Parser, chunked_simple) {
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

  std::string body;
  const bool result = response::Parser::parse(
      message.begin(), message.end(), body
  );

  ASSERT_EQ(result, true);
  ASSERT_STREQ("ABC", body.c_str());
}

TEST_F(Parser, chunked_forbidden) {
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

  std::string body;
  ASSERT_THROW(
      response::Parser::parse(message.begin(), message.end(), body),
      std::runtime_error
  );
}

TEST_F(Parser, chunked_real) {
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

  std::string body;
  const bool result = response::Parser::parse(
      message.begin(), message.end(), body
  );

  ASSERT_EQ(result, true);
}

} // namespace unittest
} // namespace response
} // namespace http
} // namespace network
} // namespace sober