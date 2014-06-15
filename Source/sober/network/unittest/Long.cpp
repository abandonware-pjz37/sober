#include <leathers/push>
#include <leathers/all>
# include <gtest/gtest.h> // TEST_F
# include <thread> // std::this_thread
#include <leathers/pop>

#include <sober/network/Engine.hpp>
#include <sober/network/http/Stream.hpp>
#include <sober/network/http/delegate/Retry.hpp>
#include <sober/network/http/delegate/String.hpp>
#include <sober/utils/Test.hpp>

#include <leathers/push>
#include <leathers/global-constructors>

namespace sober {
namespace network {
namespace unittest {

class Long: public utils::Test {
 public:
  Long();

  virtual ~Long();

 protected:
  network::Engine engine_;
  network::http::Stream stream_;
};

Long::Long(): stream_(engine_) {
}

Long::~Long() {
}

TEST_F(Long, queries_with_timeout) {
  class Delegate: public http::delegate::Retry, public http::delegate::String {
   public:
    Delegate():
        http::delegate::Retry(1, boost::posix_time::milliseconds(0)) {
    }
  };

  Delegate delegate;

  stream_.set_endpoint("http://pastebin.com/raw.php");
  stream_.set_delegate(delegate);

  // first request
  stream_.request.set_query("i", "A8wzq8s3");
  stream_.async_start();

  engine_.run();

  ASSERT_TRUE(delegate.ready());
  ASSERT_STREQ(delegate.body().c_str(), "Hello, request!");

  std::this_thread::sleep_for(std::chrono::seconds(60));

  stream_.clear_statistic();
  ASSERT_EQ(stream_.statistic().get_restarted(), 0);

  // second request (expected connect repeating)
  stream_.request.set_query("i", "zUyXrfcz");
  stream_.async_start();

  engine_.run();

  ASSERT_GE(stream_.statistic().get_restarted(), 1);

  ASSERT_TRUE(delegate.ready());
  ASSERT_STREQ(delegate.body().c_str(), "! One more time");
}

TEST_F(Long, operation_timeout) {
  http::delegate::String delegate;

  stream_.set_endpoint("http://111.22.33.33");
  stream_.set_delegate(delegate);
  stream_.async_start();

  engine_.run();

  ASSERT_FALSE(delegate.ready());
}

} // namespace unittest
} // namespace network
} // namespace sober

#include <leathers/pop>
