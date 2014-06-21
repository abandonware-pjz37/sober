#ifndef SOBER_LOG_ATTRIBUTES_HPP_
#define SOBER_LOG_ATTRIBUTES_HPP_

// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <leathers/push>
#include <leathers/all>
# include <boost/log/expressions/keyword.hpp> // BOOST_LOG_ATTRIBUTE_KEYWORD
#include <leathers/pop>

#include <sober/log/Severity.hpp>

namespace sober {
namespace log {

BOOST_LOG_ATTRIBUTE_KEYWORD(id, "sober.id", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "sober.severity", Severity)
BOOST_LOG_ATTRIBUTE_KEYWORD(this_, "sober.this", const void*)

void base_formatter(
    const boost::log::record_view& rec, boost::log::formatting_ostream& stream
);

void formatter(
    const boost::log::record_view& rec, boost::log::formatting_ostream& stream
);

} // namespace log
} // namespace sober

#endif // SOBER_LOG_ATTRIBUTES_HPP_
