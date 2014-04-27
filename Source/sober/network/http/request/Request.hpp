#ifndef SOBER_NETWORK_TCP_HTTP_REQUEST_
#define SOBER_NETWORK_TCP_HTTP_REQUEST_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <string>

namespace sober {
namespace network {
namespace http {
namespace request {

class Request {
 public:
  static Request make_get(
      const std::string& host_name,
      const std::string& uri
  );

  const std::string& str() const noexcept;

 private:
  static Request make_1_1(
      const char* method,
      const std::string& host_name,
      const std::string& uri
  );

  std::string request_;
};

} // namespace request
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_TCP_HTTP_REQUEST_
