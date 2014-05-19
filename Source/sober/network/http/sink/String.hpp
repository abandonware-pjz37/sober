#ifndef SOBER_NETWORK_HTTP_SINK_STRING_HPP_
#define SOBER_NETWORK_HTTP_SINK_STRING_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/Sink.hpp>

namespace sober {
namespace network {
namespace http {
namespace sink {

class String: public Sink {
 public:
  String() noexcept;

  void clear() override noexcept;

  /**
    * @throws @c std::runtime_error if already finished
    */
  void write(char* buffer, std::size_t size, bool finish) override;

  bool ready() const noexcept;

  /**
    * @throws @c std::runtime_error if body not ready
    */
  const std::string& body() const;

 private:
  bool ready_;
  std::string body_;
};

} // namespace sink
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_SINK_STRING_HPP_
