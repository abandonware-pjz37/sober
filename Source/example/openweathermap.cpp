// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/sober.hpp>

class Delegate : public sober::network::api::OpenWeatherMap {
 public:
  using Base = sober::network::api::OpenWeatherMap;
  using StatusCode = sober::network::http::response::attribute::StatusCode;
  using ErrorCode = boost::system::error_code;

  Delegate(sober::network::http::Stream& stream):
      Base(stream),
      max_count_(10) {
  }

  virtual void on_start() override;

  virtual boost::posix_time::time_duration watchdog_period() override {
    return boost::posix_time::milliseconds(1000);
  }

  virtual bool force_stop() override {
    ++counter_;
    std::cout << "Count " << counter_ << "/" << max_count_ << std::endl;
    bool stop = (counter_ >= max_count_);
    if (stop) {
      std::cerr << "Timeout reached" << std::endl;
    }
    return stop;
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
    std::cout << "Success..." << std::endl;
    std::cout << "  longitude: " << attr.longitude << std::endl;
    std::cout << "  latitude: " << attr.latitude << std::endl;
    std::cout << "  temperature: " << attr.temperature << std::endl;
    std::cout << "  temperature(human): " << attr.temperature_human
        << std::endl;
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

void Delegate::on_start() {
  counter_ = 0;
}

int main() {
  try {
    const bool debug = !sober::utils::always();

    if (debug) {
      sober::log::Logger::init_logs_for_testing(true, true);
    }
    else {
      sober::log::Logger::init_logs_for_testing(false, false);
    }

    namespace net = sober::network;

    net::Engine engine;
    net::http::Stream stream(engine);

    Delegate delegate(stream);

    const char* city = "Madrid";
    std::cout << "City: " << city << std::endl;

    delegate.async_get_city(city);

    engine.run();
    return sober::Exit::OK;
  }
  catch (std::exception& exc) {
    std::cerr << "Exception: " << exc.what() << std::endl;
    return sober::Exit::FAIL;
  }
}
