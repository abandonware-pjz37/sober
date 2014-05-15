#include <gtest/gtest.h> // TEST_F
#include <sober/network/Engine.hpp>
#include <sober/network/http/Stream.hpp>
#include <sober/network/http/delegate/Retry.hpp>
#include <thread> // std::this_thread

namespace sober {
namespace network {
namespace unittest {

class Long: public ::testing::Test {
 public:
  Long(): stream_(engine_) {
  }

 protected:
  network::Engine engine_;
  network::http::Stream stream_;
};

TEST_F(Long, queries_with_timeout) {
  http::delegate::Retry delegate(1, boost::posix_time::milliseconds(0));

  stream_.set_endpoint("http://pastebin.com/raw.php");
  stream_.set_delegate(delegate);

  // first request
  stream_.set_query("i", "A8wzq8s3");
  stream_.push_request();
  engine_.run();

  ASSERT_TRUE(stream_.response_valid());
  ASSERT_STREQ(stream_.response_body().c_str(), "Hello, request!");

  std::this_thread::sleep_for(std::chrono::seconds(60));

  stream_.clear_statistic();
  ASSERT_EQ(stream_.statistic().get_restarted(), 0);

  engine_.run();

  ASSERT_GE(stream_.statistic().get_restarted(), 1);

  // second request (repeat connect expected)
  stream_.set_query("i", "zUyXrfcz");
  stream_.push_request();
  engine_.run();

  ASSERT_TRUE(stream_.response_valid());
  ASSERT_STREQ(stream_.response_body().c_str(), "! One more time");
}

TEST_F(Long, operation_timeout) {
  stream_.set_endpoint("http://111.22.33.33");
  stream_.push_request();

  engine_.run();

  ASSERT_FALSE(stream_.response_valid());
}

} // namespace unittest
} // namespace network
} // namespace sober
