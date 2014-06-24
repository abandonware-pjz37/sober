// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <leathers/push>
#include <leathers/all>
# include <boost/log/sources/record_ostream.hpp> // BOOST_LOG
# include <chrono>
# include <gtest/gtest.h> // TEST_F
#include <leathers/pop>

#include <sober/network/Engine.hpp>
#include <sober/network/http/Stream.hpp>
#include <sober/network/http/delegate/Reconnect.hpp>
#include <sober/network/http/delegate/Retry.hpp>
#include <sober/network/http/delegate/Timeout.hpp>
#include <sober/network/http/delegate/String.hpp>
#include <sober/utils/Test.hpp>
#include <sober/utils/run_duration.hpp>

#include <leathers/push>
#include <leathers/global-constructors>

namespace sober {
namespace network {
namespace unittest {

class Engine: public utils::Test {
 public:
  Engine():
      stream_(engine_) {
  }

  virtual ~Engine() override;

 protected:
  network::Engine engine_;
  network::http::Stream stream_;
};

Engine::~Engine() {
}

TEST_F(Engine, simple) {
  http::delegate::String delegate;
  stream_.set_delegate(delegate);

  stream_.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");
  stream_.async_start();

  engine_.run();

  ASSERT_TRUE(delegate.ready());
  ASSERT_STREQ(delegate.body().c_str(), "Hello, request!");
  ASSERT_EQ(
      stream_.response.header().status_line.status_code,
      http::response::attribute::StatusCode::OK
  );
}

TEST_F(Engine, simple_twice) {
  http::delegate::String delegate;
  stream_.set_delegate(delegate);

  stream_.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");
  stream_.async_start();
  engine_.run();
  ASSERT_TRUE(delegate.ready());
  ASSERT_STREQ(delegate.body().c_str(), "Hello, request!");
  ASSERT_EQ(
      stream_.response.header().status_line.status_code,
      http::response::attribute::StatusCode::OK
  );

  stream_.async_start();
  ASSERT_FALSE(delegate.ready());

  engine_.run();
  ASSERT_TRUE(delegate.ready());
  ASSERT_STREQ(delegate.body().c_str(), "Hello, request!");
  ASSERT_EQ(
      stream_.response.header().status_line.status_code,
      http::response::attribute::StatusCode::OK
  );
}

TEST_F(Engine, simple_query) {
  http::delegate::String delegate;
  stream_.set_delegate(delegate);

  stream_.set_endpoint("http://pastebin.com/raw.php");
  stream_.request.set_query("i", "A8wzq8s3");
  stream_.async_start();

  namespace ch = std::chrono;

  auto duration = utils::run_duration<ch::high_resolution_clock>(
      [&]() { engine_.run(); }
  );

  ASSERT_LT(ch::duration_cast<ch::seconds>(duration).count(), 2);

  ASSERT_TRUE(delegate.ready());
  ASSERT_STREQ(delegate.body().c_str(), "Hello, request!");
  ASSERT_EQ(
      stream_.response.header().status_line.status_code,
      http::response::attribute::StatusCode::OK
  );
}

TEST_F(Engine, multiple_query) {
  http::delegate::String delegate;
  stream_.set_delegate(delegate);

  stream_.set_endpoint("http://pastebin.com/raw.php");

  // first request
  stream_.request.set_query("i", "A8wzq8s3");
  stream_.async_start();
  engine_.run();

  ASSERT_TRUE(delegate.ready());
  ASSERT_STREQ(delegate.body().c_str(), "Hello, request!");
  ASSERT_EQ(
      stream_.response.header().status_line.status_code,
      http::response::attribute::StatusCode::OK
  );

  // second request
  stream_.request.set_query("i", "zUyXrfcz");
  stream_.async_start();
  engine_.run();

  ASSERT_TRUE(delegate.ready());
  ASSERT_STREQ(delegate.body().c_str(), "! One more time");
  ASSERT_EQ(
      stream_.response.header().status_line.status_code,
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
}

TEST_F(Engine, retry_with_timeout) {
  enum { RETRY_NUMBER = 2 };

  using StatusCode = http::response::attribute::StatusCode;

  class RetryWithTimeout:
      public http::delegate::Retry,
      public http::delegate::Timeout {
   public:
    RetryWithTimeout():
        http::delegate::Retry(RETRY_NUMBER, boost::posix_time::millisec(10)),
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

  ASSERT_EQ(stream_.statistic().get_restarted(), RETRY_NUMBER);
}

TEST_F(Engine, default_delegate) {
  http::delegate::Interface delegate;

  stream_.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");
  stream_.set_delegate(delegate);
  stream_.async_start();

  engine_.run();
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
}

TEST_F(Engine, success_handler) {
  class Delegate : public http::delegate::String {
   public:
    Delegate() BOOST_NOEXCEPT: done_(false) {
    }

    void on_success() BOOST_NOEXCEPT override {
      done_ = true;
      ASSERT_STREQ(body().c_str(), "Hello, request!");
    }

    bool done() const BOOST_NOEXCEPT {
      return done_;
    }

   private:
    bool done_;
  };

  Delegate delegate;

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

  std::string req_result;

  class Delegate : public http::delegate::String {
   public:
    Delegate(http::Stream& stream, std::string& result) BOOST_NOEXCEPT:
        stream_(stream),
        first_done_(false),
        result_(result) {
    }

    void on_success() BOOST_NOEXCEPT override {
      if (first_done_) {
        result_ = body();
        return;
      }

      ASSERT_STREQ(body().c_str(), "A8wzq8s3");
      first_done_ = true;
      stream_.request.set_query("i", body());
      stream_.async_start();
    }

   private:
    http::Stream& stream_;
    bool first_done_;
    std::string& result_;
  };

  Delegate delegate(stream_, req_result);
  stream_.set_delegate(delegate);
  stream_.set_endpoint("http://pastebin.com/raw.php?i=Peb93awR");
  stream_.async_start();

  engine_.run();

  ASSERT_STREQ(req_result.c_str(), "Hello, request!");
}

TEST_F(Engine, race) {
  class Delegate: public http::delegate::String {
   public:
    Delegate(const char* name, bool& win): name_(name), win_(win) {
    }

    void on_success() override {
      if (!win_) {
        win_ = true;
        BOOST_LOG(log_) << name_ << " win";
      }
    }

   private:
    const char* name_;
    bool& win_;
    boost::log::sources::logger log_;
  };

  // No need for mutex protection
  bool win = false;

  Delegate delegate_A("A", win), delegate_B("B", win);

  http::Stream stream_A(engine_);
  http::Stream stream_B(engine_);

  stream_A.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");
  stream_B.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");

  stream_A.set_delegate(delegate_A);
  stream_B.set_delegate(delegate_B);

  stream_A.async_start();
  stream_B.async_start();

  ASSERT_FALSE(win);
  engine_.run();
  ASSERT_TRUE(win);

  ASSERT_TRUE(delegate_A.ready());
  ASSERT_TRUE(delegate_B.ready());

  ASSERT_STREQ(delegate_A.body().c_str(), "Hello, request!");
  ASSERT_STREQ(delegate_B.body().c_str(), "Hello, request!");

  ASSERT_EQ(
      stream_A.response.header().status_line.status_code,
      http::response::attribute::StatusCode::OK
  );
  ASSERT_EQ(
      stream_B.response.header().status_line.status_code,
      http::response::attribute::StatusCode::OK
  );
}

TEST_F(Engine, race_with_cancel) {
  class Delegate: public http::delegate::String {
   public:
    Delegate(const char* name, http::Stream& other):
        name_(name),
        other_(other) {
    }

    void on_success() override {
      BOOST_LOG(log_) << name_ << " win";
      other_.cancel();
    }

   private:
    const char* name_;
    boost::log::sources::logger log_;
    http::Stream& other_;
  };

  http::Stream stream_A(engine_);
  http::Stream stream_B(engine_);

  stream_A.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");
  stream_B.set_endpoint("http://pastebin.com/raw.php?i=A8wzq8s3");

  Delegate delegate_A("A", stream_B), delegate_B("B", stream_A);

  stream_A.set_delegate(delegate_A);
  stream_B.set_delegate(delegate_B);

  stream_A.async_start();
  stream_B.async_start();

  engine_.run();

  ASSERT_EQ(delegate_A.ready() + delegate_B.ready(), 1);

  http::delegate::String& sink_good =
      delegate_A.ready() ? delegate_A : delegate_B;
  http::Stream& stream_good = delegate_A.ready() ? stream_A : stream_B;

  ASSERT_STREQ(sink_good.body().c_str(), "Hello, request!");
  ASSERT_EQ(
      stream_good.response.header().status_line.status_code,
      http::response::attribute::StatusCode::OK
  );
}

TEST_F(Engine, redirect) {
  class Delegate: public http::delegate::String {
   public:
    virtual bool allow_redirection() override {
      return true;
    }
  };

  Delegate delegate;

  stream_.set_delegate(delegate);

  stream_.set_endpoint("http://www.rzd.ru");
  stream_.async_start();

  engine_.run();

  ASSERT_TRUE(delegate.ready());
  ASSERT_EQ(
      stream_.response.header().status_line.status_code,
      http::response::attribute::StatusCode::OK
  );
}

TEST_F(Engine, created) {
  class Delegate: public http::delegate::String {
   public:
    Delegate(): success_(false) {
    }

    virtual void on_success() override {
      ASSERT_FALSE(success_);
      success_ = true;
    }

    bool success() const {
      return success_;
    }

   private:
    bool success_;
  };

  Delegate delegate;
  stream_.set_delegate(delegate);

  stream_.set_endpoint(
      "http://pass.rzd.ru/suggester?lang=ru&stationNamePart=gg"
  );
  stream_.async_start();

  engine_.run();

  ASSERT_TRUE(delegate.success());
  ASSERT_EQ(
      stream_.response.header().status_line.status_code,
      http::response::attribute::StatusCode::CREATED
  );
}

} // namespace unittest
} // namespace network
} // namespace sober

#include <leathers/pop>
