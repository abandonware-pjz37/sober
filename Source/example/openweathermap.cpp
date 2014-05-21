// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/sober.hpp>

class Delegate : sober::network::api::OpenWeatherMap {
 public:
  using Base = sober::network::api::OpenWeatherMap;
  using StatusCode = sober::network::http::response::attribute::StatusCode;
  using ErrorCode = boost::system::error_code;

  Delegate(): max_count_(10) {
  }

  virtual void on_start() override {
    counter_ = 0;
  }

  virtual boost::posix_time::time_duration watchdog_period() override {
    return boost::posix_time::milliseconds(100);
  }

  virtual bool force_stop() override {
    ++counter_;
    std::cout << "Count " << counter_ << "/" << max_count_ << std::endl;
    return counter_ >= max_count_;
  }

  virtual void on_success() override {
    try {
      Base::on_success();
    }
    catch (const std::exception& exc) {
      std::cerr << "Parse failed: " << exc.what() << std::endl;
      return;
    }
    const Attribute& attr = attribute();
    std::cout << "OK:" << std::endl;
    std::cout << "  longitude: " << attr.longitude << std::endl;
    std::cout << "  latitude: " << attr.latitude << std::endl;
    std::cout << "  description: " << attr.description << std::endl;
    std::cout << "  icon: " << attr.icon << std::endl;
  }

  virtual bool restart_on_error(const ErrorCode& error) override {
    std::cerr << "Error: " << error.message() << std::endl;
    return false;
  }

  virtual bool restart_on_error(const StatusCode& status_code) override {
    std::cerr << "Error(bad status code): " << status_code << std::endl;
    return false;
  }

 private:
  const int max_count_;
  int counter_;
};

int main() {
  const bool debug = true;

  if (debug) {
    sober::utils::Test::init_logs(true, true);
  }
  else {
    sober::utils::Test::init_logs(false, false);
  }

  namespace net = sober::network;

  net::Engine engine;
  net::http::Stream stream(engine);
  net::api::OpenWeatherMap delegate;

  stream.set_endpoint("http://api.openweathermap.org/data/2.5/weather");
  stream.set_delegate(delegate);

  stream.request.set_query("q", "Madrid");
  stream.async_start();

  engine.run();
}
