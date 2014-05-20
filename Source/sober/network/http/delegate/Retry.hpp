#ifndef SOBER_NETWORK_HTTP_DELEGATE_RETRY_HPP_
#define SOBER_NETWORK_HTTP_DELEGATE_RETRY_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/delegate/Interface.hpp>

namespace sober {
namespace network {
namespace http {
namespace delegate {

class Retry: virtual public Interface {
 public:
  using Duration = boost::posix_time::time_duration;

  Retry(unsigned times, Duration pause_duration);

  virtual void on_start() override;
  virtual bool restart_on_error(const boost::system::error_code&) override;
  virtual bool restart_on_error(
      const response::attribute::StatusCode&
  ) override;
  virtual boost::posix_time::time_duration restart_pause() override;

 private:
  bool restart_on_error();

  const unsigned init_times_;
  const Duration pause_duration_;

  unsigned times_;
};

} // namespace delegate
} // namespace http
} // namespace network
} // namespace sober

#endif // SOBER_NETWORK_HTTP_DELEGATE_RETRY_HPP_
