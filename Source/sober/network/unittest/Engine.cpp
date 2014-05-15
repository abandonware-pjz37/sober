// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <chrono>
#include <gtest/gtest.h> // TEST_F
#include <sober/network/Engine.hpp>
#include <sober/network/http/Stream.hpp>
#include <sober/network/http/delegate/Reconnect.hpp>
#include <sober/network/http/delegate/Retry.hpp>
#include <sober/network/http/delegate/Timeout.hpp>
#include <sober/utils/Test.hpp>
#include <sober/utils/run_duration.hpp>

namespace sober {
namespace network {
namespace unittest {

class Engine: public utils::Test {
 public:
  Engine(): stream_(engine_) {
  }

 protected:
  network::Engine engine_;
  network::http::Stream stream_;
};

TEST_F(Engine, simple) {
  stream_.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");
  stream_.push_request();

  engine_.run();

  ASSERT_TRUE(stream_.response_valid());
  ASSERT_STREQ(stream_.response_body().c_str(), "Hello, request!");
}

TEST_F(Engine, simple_twice) {
  stream_.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");
  stream_.push_request();
  engine_.run();
  ASSERT_TRUE(stream_.response_valid());
  ASSERT_STREQ(stream_.response_body().c_str(), "Hello, request!");

  stream_.push_request();
  ASSERT_FALSE(stream_.response_valid());

  engine_.run();
  ASSERT_TRUE(stream_.response_valid());
  ASSERT_STREQ(stream_.response_body().c_str(), "Hello, request!");
}

TEST_F(Engine, simple_query) {
  stream_.set_endpoint("http://pastebin.com/raw.php");
  stream_.set_query("i", "A8wzq8s3");
  stream_.push_request();

  namespace ch = std::chrono;

  auto duration = utils::run_duration<ch::high_resolution_clock>(
      [&]() { engine_.run(); }
  );

  ASSERT_LT(ch::duration_cast<ch::seconds>(duration).count(), 2);

  ASSERT_TRUE(stream_.response_valid());
  ASSERT_STREQ(stream_.response_body().c_str(), "Hello, request!");
}

TEST_F(Engine, multiple_query) {
  stream_.set_endpoint("http://pastebin.com/raw.php");

  // first request
  stream_.set_query("i", "A8wzq8s3");
  stream_.push_request();
  engine_.run();

  ASSERT_TRUE(stream_.response_valid());
  ASSERT_STREQ(stream_.response_body().c_str(), "Hello, request!");

  // second request
  stream_.set_query("i", "zUyXrfcz");
  stream_.push_request();
  engine_.run();

  ASSERT_TRUE(stream_.response_valid());
  ASSERT_STREQ(stream_.response_body().c_str(), "! One more time");
}

TEST_F(Engine, operation_timeout) {
  http::delegate::Timeout delegate(boost::posix_time::seconds(2));

  stream_.set_endpoint("http://111.22.33.33");
  stream_.set_delegate(delegate);
  stream_.push_request();

  namespace ch = std::chrono;

  auto duration = sober::utils::run_duration<ch::high_resolution_clock>(
      [&]() { engine_.run(); }
  );

  ASSERT_LT(ch::duration_cast<ch::seconds>(duration).count(), 3);
  ASSERT_FALSE(stream_.response_valid());
  ASSERT_ANY_THROW(stream_.response_body());
}

TEST_F(Engine, retry_on_error) {
  const unsigned retry_number = 3;
  http::delegate::Retry delegate(retry_number, boost::posix_time::seconds(1));

  stream_.set_endpoint("http://pastebin.com/A8wzq8s5"); // not exists
  stream_.set_delegate(delegate);
  stream_.push_request();

  stream_.clear_statistic();
  ASSERT_EQ(stream_.statistic().get_restarted(), 0);

  engine_.run();

  ASSERT_EQ(stream_.statistic().get_restarted(), retry_number);

  ASSERT_FALSE(stream_.response_valid());
}

TEST_F(Engine, retry_with_timeout) {
  const unsigned retry_number = 2;
  class RetryWithTimeout:
      public http::delegate::Retry,
      public http::delegate::Timeout {
   public:
    RetryWithTimeout():
        http::delegate::Retry(retry_number, boost::posix_time::seconds(1)),
        http::delegate::Timeout(boost::posix_time::seconds(3)) {
    }
  };

  RetryWithTimeout delegate;

  stream_.set_endpoint("http://pastebin.com/A8wzq8s5"); // not exists
  stream_.set_delegate(delegate);
  stream_.push_request();

  stream_.clear_statistic();
  ASSERT_EQ(stream_.statistic().get_restarted(), 0);

  engine_.run();

  ASSERT_EQ(stream_.statistic().get_restarted(), retry_number);

  ASSERT_FALSE(stream_.response_valid());
}

TEST_F(Engine, default_delegate) {
  http::delegate::Interface delegate;

  stream_.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");
  stream_.set_delegate(delegate);
  stream_.push_request();

  engine_.run();

  ASSERT_TRUE(stream_.response_valid());
  ASSERT_STREQ(stream_.response_body().c_str(), "Hello, request!");
}

TEST_F(Engine, reconnect_delegate) {
  // Try connect for 5 ms (expected error and exit)
  http::delegate::Reconnect delegate(boost::posix_time::milliseconds(5));

  stream_.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");
  stream_.set_delegate(delegate);
  stream_.push_request();

  stream_.clear_statistic();
  ASSERT_EQ(stream_.statistic().get_reconnect(), 0);

  engine_.run();

  ASSERT_EQ(stream_.statistic().get_reconnect(), 1);
  ASSERT_FALSE(stream_.response_valid());
}

TEST_F(Engine, success_handler) {
  class Delegate : public http::delegate::Interface {
   public:
    Delegate() noexcept: done_(false) {
    }

    void on_success(const std::string& body) noexcept override {
      done_ = true;
      ASSERT_STREQ(body.c_str(), "Hello, request!");
    }

    bool done() const noexcept {
      return done_;
    }
   private:
    bool done_;
  };

  Delegate delegate;

  stream_.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");
  stream_.set_delegate(delegate);
  stream_.push_request();

  ASSERT_FALSE(delegate.done());
  engine_.run();
  ASSERT_TRUE(delegate.done());
}

TEST_F(Engine, chain_request) {
  // Read first request and start second if first successful
  // 1: http://pastebin.com/raw.php?i=Peb93awR -> "A8wzq8s3"
  // 2: http://pastebin.com/raw.php?i=A8wzq8s3 -> "Hello, request!"

  std::string result;

  class Delegate : public http::delegate::Interface {
   public:
    Delegate(http::Stream& stream, std::string& result) noexcept:
        stream_(stream),
        first_done_(false),
        result_(result) {
    }

    void on_success(const std::string& body) noexcept override {
      if (first_done_) {
        result_ = body;
        return;
      }

      ASSERT_STREQ(body.c_str(), "A8wzq8s3");
      first_done_ = true;
      stream_.set_query("i", body);
      stream_.push_request();
    }

   private:
    http::Stream& stream_;
    bool first_done_;
    std::string& result_;
  };

  Delegate delegate(stream_, result);
  stream_.set_delegate(delegate);
  stream_.set_endpoint("http://pastebin.com/raw.php?i=Peb93awR");
  stream_.push_request();

  engine_.run();

  ASSERT_STREQ(result.c_str(), "Hello, request!");
}

} // namespace unittest
} // namespace network
} // namespace sober
