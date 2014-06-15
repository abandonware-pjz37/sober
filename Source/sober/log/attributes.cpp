// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/log/attributes.hpp>

#include <leathers/push>
#include <leathers/all>
# include <boost/date_time.hpp> // boost::posix_time
# include <boost/log/attributes/current_thread_id.hpp>
# include <boost/log/detail/default_attribute_names.hpp>
# include <boost/log/detail/timestamp.hpp>
# include <boost/log/expressions/formatters.hpp>
# include <boost/log/expressions/message.hpp>
# include <boost/log/utility/formatting_ostream.hpp>
#include <leathers/pop>

namespace sober {
namespace log {

void base_formatter(
    const boost::log::record_view& rec, boost::log::formatting_ostream& stream
) {
  namespace expr = boost::log::expressions;

  const boost::log::attribute_value_set& values = rec.attribute_values();
  const auto it = values.find("sober.id");
  if (it == values.end()) {
    // this is not a 'sober' record
    return;
  }

  stream << "[" << rec[id] << " this:" << rec[this_] << "]";
  if (rec[severity] == Severity::DEBUG) {
    stream << " !!" << rec[severity] << "!!";
  }
}

void formatter(
    const boost::log::record_view& rec, boost::log::formatting_ostream& stream
) {
  namespace log = boost::log;
  namespace expr = log::expressions;
  namespace attrs = log::attributes;

  stream << "[#"
      << std::setfill('0') << std::setw(6)
      << log::extract<unsigned int>("LineID", rec) << " "
      << log::extract<boost::posix_time::ptime>("TimeStamp", rec)
      << " thread:"
      << log::extract<attrs::current_thread_id::value_type>("ThreadID", rec)
      << "] "
  ;

  base_formatter(rec, stream);

  stream << " " << rec[expr::smessage];
}

} // namespace log
} // namespace sober
