#include <gtest/gtest.h> // TEST_F
#include <sober/network/Engine.hpp>
#include <sober/network/http/Stream.hpp>
#include <sober/network/http/delegate/Retry.hpp>
#include <sober/network/http/sink/String.hpp>
#include <sober/utils/Test.hpp>
#include <thread> // std::this_thread

namespace sober {
namespace network {
namespace unittest {

class Long: public utils::Test {
 public:
  static const std::size_t BUFFER_SIZE = 1024;

  Long(): response_(BUFFER_SIZE, sink_), stream_(engine_, request_, response_) {
  }

 protected:
  http::sink::String sink_;
  http::Response response_;
  http::Request request_;
  network::Engine engine_;
  network::http::Stream stream_;
};

TEST_F(Long, queries_with_timeout) {
  http::delegate::Retry delegate(1, boost::posix_time::milliseconds(0));

  stream_.set_endpoint("http://pastebin.com/raw.php");
  stream_.set_delegate(delegate);

  // first request
  request_.set_query("i", "A8wzq8s3");
  stream_.async_start();

  engine_.run();

  ASSERT_TRUE(sink_.ready());
  ASSERT_STREQ(sink_.body().c_str(), "Hello, request!");

  std::this_thread::sleep_for(std::chrono::seconds(60));

  stream_.clear_statistic();
  ASSERT_EQ(stream_.statistic().get_restarted(), 0);

  // second request (expected connect repeating)
  request_.set_query("i", "zUyXrfcz");
  stream_.async_start();

  engine_.run();

  ASSERT_GE(stream_.statistic().get_restarted(), 1);

  ASSERT_TRUE(sink_.ready());
  ASSERT_STREQ(sink_.body().c_str(), "! One more time");
}

TEST_F(Long, operation_timeout) {
  stream_.set_endpoint("http://111.22.33.33");
  stream_.async_start();

  engine_.run();

  ASSERT_FALSE(sink_.ready());
}

} // namespace unittest
} // namespace network
} // namespace sober
