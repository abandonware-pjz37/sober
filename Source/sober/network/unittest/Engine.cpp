// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <chrono>
#include <gtest/gtest.h> // TEST_F
#include <sober/network/Engine.hpp>
#include <sober/network/http/Stream.hpp>
#include <sober/network/http/delegate/Reconnect.hpp>
#include <sober/network/http/delegate/Retry.hpp>
#include <sober/network/http/delegate/Timeout.hpp>
#include <sober/network/http/sink/String.hpp>
#include <sober/utils/Test.hpp>
#include <sober/utils/run_duration.hpp>

namespace sober {
namespace network {
namespace unittest {

class Engine: public utils::Test {
 public:
  Engine():
      response_(RESPONSE_BUFFER_SIZE, sink_),
      stream_(engine_, request_, response_) {
  }

 protected:
  static const std::size_t RESPONSE_BUFFER_SIZE = 2048;

  network::Engine engine_;
  http::sink::String sink_;
  http::Response response_;
  http::Request request_;
  network::http::Stream stream_;
};

TEST_F(Engine, simple) {
  stream_.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");
  stream_.async_start();

  engine_.run();

  ASSERT_TRUE(sink_.ready());
  ASSERT_STREQ(sink_.body().c_str(), "Hello, request!");
  ASSERT_EQ(
      response_.header().status_line.status_code,
      http::response::attribute::StatusCode::OK
  );
}

TEST_F(Engine, simple_twice) {
  stream_.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");
  stream_.async_start();
  engine_.run();
  ASSERT_TRUE(sink_.ready());
  ASSERT_STREQ(sink_.body().c_str(), "Hello, request!");
  ASSERT_EQ(
      response_.header().status_line.status_code,
      http::response::attribute::StatusCode::OK
  );

  stream_.async_start();
  ASSERT_FALSE(sink_.ready());

  engine_.run();
  ASSERT_TRUE(sink_.ready());
  ASSERT_STREQ(sink_.body().c_str(), "Hello, request!");
  ASSERT_EQ(
      response_.header().status_line.status_code,
      http::response::attribute::StatusCode::OK
  );
}

TEST_F(Engine, simple_query) {
  stream_.set_endpoint("http://pastebin.com/raw.php");
  request_.set_query("i", "A8wzq8s3");
  stream_.async_start();

  namespace ch = std::chrono;

  auto duration = utils::run_duration<ch::high_resolution_clock>(
      [&]() { engine_.run(); }
  );

  ASSERT_LT(ch::duration_cast<ch::seconds>(duration).count(), 2);

  ASSERT_TRUE(sink_.ready());
  ASSERT_STREQ(sink_.body().c_str(), "Hello, request!");
  ASSERT_EQ(
      response_.header().status_line.status_code,
      http::response::attribute::StatusCode::OK
  );
}

TEST_F(Engine, multiple_query) {
  stream_.set_endpoint("http://pastebin.com/raw.php");

  // first request
  request_.set_query("i", "A8wzq8s3");
  stream_.async_start();
  engine_.run();

  ASSERT_TRUE(sink_.ready());
  ASSERT_STREQ(sink_.body().c_str(), "Hello, request!");
  ASSERT_EQ(
      response_.header().status_line.status_code,
      http::response::attribute::StatusCode::OK
  );

  // second request
  request_.set_query("i", "zUyXrfcz");
  stream_.async_start();
  engine_.run();

  ASSERT_TRUE(sink_.ready());
  ASSERT_STREQ(sink_.body().c_str(), "! One more time");
  ASSERT_EQ(
      response_.header().status_line.status_code,
      http::response::attribute::StatusCode::OK
  );
}

TEST_F(Engine, operation_timeout) {
  http::delegate::Timeout delegate(boost::posix_time::seconds(2));

  stream_.set_endpoint("http://111.22.33.33");
  stream_.set_delegate(delegate);
  stream_.async_start();

  namespace ch = std::chrono;

  auto duration = sober::utils::run_duration<ch::high_resolution_clock>(
      [&]() { engine_.run(); }
  );

  ASSERT_LT(ch::duration_cast<ch::seconds>(duration).count(), 3);
  ASSERT_FALSE(sink_.ready());
  ASSERT_THROW(sink_.body(), std::runtime_error);
}

TEST_F(Engine, retry_on_error) {
  const unsigned retry_number = 3;
  http::delegate::Retry delegate(retry_number, boost::posix_time::seconds(1));

  stream_.set_endpoint("http://pastebin.com/A8wzq8s5"); // not exists
  stream_.set_delegate(delegate);
  stream_.async_start();

  stream_.clear_statistic();
  ASSERT_EQ(stream_.statistic().get_restarted(), 0);

  engine_.run();

  ASSERT_EQ(stream_.statistic().get_restarted(), retry_number);

  ASSERT_TRUE(sink_.ready());
  ASSERT_EQ(
      response_.header().status_line.status_code,
      http::response::attribute::StatusCode::NOT_FOUND
  );
}

TEST_F(Engine, retry_with_timeout) {
  const unsigned retry_number = 2;

  class RetryWithTimeout:
      public http::delegate::Retry,
      public http::delegate::Timeout {
   public:
    RetryWithTimeout():
        http::delegate::Retry(retry_number, boost::posix_time::millisec(10)),
        http::delegate::Timeout(boost::posix_time::seconds(10)) {
    }
  };

  RetryWithTimeout delegate;

  stream_.set_endpoint("http://pastebin.com/A8wzq8s5"); // not exists
  stream_.set_delegate(delegate);
  stream_.async_start();

  stream_.clear_statistic();
  ASSERT_EQ(stream_.statistic().get_restarted(), 0);

  engine_.run();

  ASSERT_EQ(stream_.statistic().get_restarted(), retry_number);
  ASSERT_TRUE(sink_.ready());
  ASSERT_EQ(
      response_.header().status_line.status_code,
      http::response::attribute::StatusCode::NOT_FOUND
  );
}

TEST_F(Engine, default_delegate) {
  http::delegate::Interface delegate;

  stream_.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");
  stream_.set_delegate(delegate);
  stream_.async_start();

  engine_.run();

  ASSERT_TRUE(sink_.ready());
  ASSERT_STREQ(sink_.body().c_str(), "Hello, request!");
  ASSERT_EQ(
      response_.header().status_line.status_code,
      http::response::attribute::StatusCode::OK
  );
}

TEST_F(Engine, reconnect_delegate) {
  // Try connect for 5 ms (expected error and exit)
  http::delegate::Reconnect delegate(boost::posix_time::milliseconds(5));

  stream_.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");
  stream_.set_delegate(delegate);
  stream_.async_start();

  stream_.clear_statistic();
  ASSERT_EQ(stream_.statistic().get_reconnect(), 0);

  engine_.run();

  ASSERT_EQ(stream_.statistic().get_reconnect(), 1);
  ASSERT_FALSE(sink_.ready());
}

TEST_F(Engine, success_handler) {
  class Delegate : public http::delegate::Interface {
   public:
    using Sink = http::sink::String;

    Delegate(const Sink& sink) noexcept: sink_(sink), done_(false) {
    }

    void on_success() noexcept override {
      done_ = true;
      ASSERT_STREQ(sink_.body().c_str(), "Hello, request!");
    }

    bool done() const noexcept {
      return done_;
    }

   private:
    const Sink& sink_;
    bool done_;
  };

  Delegate delegate(sink_);

  stream_.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");
  stream_.set_delegate(delegate);
  stream_.async_start();

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
    using Sink = http::sink::String;

    Delegate(
        http::Stream& stream,
        http::Request& request,
        const Sink& sink,
        std::string& result
    ) noexcept:
        stream_(stream),
        request_(request),
        sink_(sink),
        first_done_(false),
        result_(result) {
    }

    void on_success() noexcept override {
      if (first_done_) {
        result_ = sink_.body();
        return;
      }

      ASSERT_STREQ(sink_.body().c_str(), "A8wzq8s3");
      first_done_ = true;
      request_.set_query("i", sink_.body());
      stream_.async_start();
    }

   private:
    http::Stream& stream_;
    http::Request& request_;
    const Sink& sink_;

    bool first_done_;
    std::string& result_;
  };

  Delegate delegate(stream_, request_, sink_, result);
  stream_.set_delegate(delegate);
  stream_.set_endpoint("http://pastebin.com/raw.php?i=Peb93awR");
  stream_.async_start();

  engine_.run();

  ASSERT_STREQ(result.c_str(), "Hello, request!");
}

} // namespace unittest
} // namespace network
} // namespace sober
