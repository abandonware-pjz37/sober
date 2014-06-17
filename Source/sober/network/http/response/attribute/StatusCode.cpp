// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/attribute/StatusCode.hpp>

#include <leathers/push>
#include <leathers/all>
# include <iostream> // std::ostream
#include <leathers/pop>

namespace sober {
namespace network {
namespace http {
namespace response {
namespace attribute {

std::ostream& operator<< (std::ostream& stream, StatusCode status_code) {
  switch (status_code) {
    case StatusCode::CONTINUE:
      stream << "Continue";
      break;
    case StatusCode::SWITCHING_PROTOCOLS:
      stream << "Switching Protocols";
      break;
    case StatusCode::PROCESSING:
      stream << "Processing";
      break;
    case StatusCode::OK:
      stream << "OK";
      break;
    case StatusCode::CREATED:
      stream << "Created";
      break;
    case StatusCode::ACCEPTED:
      stream << "Accepted";
      break;
    case StatusCode::NON_AUTHORITATIVE_INFORMATION:
      stream << "Non-Authoritative Information";
      break;
    case StatusCode::NO_CONTENT:
      stream << "No Content";
      break;
    case StatusCode::RESET_CONTENT:
      stream << "Reset Content";
      break;
    case StatusCode::PARTIAL_CONTENT:
      stream << "Partial Content";
      break;
    case StatusCode::MULTI_STATUS:
      stream << "Multi-Status";
      break;
    case StatusCode::ALREADY_REPORTED:
      stream << "Already Reported";
      break;
    case StatusCode::IM_USED:
      stream << "IM Used";
      break;
    case StatusCode::MULTIPLE_CHOICES:
      stream << "Multiple Choices";
      break;
    case StatusCode::MOVED_PERMANENTLY:
      stream << "Moved Permanently";
      break;
    case StatusCode::FOUND:
      stream << "Found";
      break;
    case StatusCode::SEE_OTHER:
      stream << "See Other";
      break;
    case StatusCode::NOT_MODIFIED:
      stream << "Not Modified";
      break;
    case StatusCode::USE_PROXY:
      stream << "Use Proxy";
      break;
    case StatusCode::SWITCH_PROXY:
      stream << "Switch Proxy";
      break;
    case StatusCode::TEMPORARY_REDIRECT:
      stream << "Temporary Redirect";
      break;
    case StatusCode::PERMANENT_REDIRECT:
      stream << "Permanent Redirect";
      break;
    case StatusCode::BAD_REQUEST:
      stream << "Bad Request";
      break;
    case StatusCode::UNAUTHORIZED:
      stream << "Unauthorized";
      break;
    case StatusCode::PAYMENT_REQUIRED:
      stream << "Payment Required";
      break;
    case StatusCode::FORBIDDEN:
      stream << "Forbidden";
      break;
    case StatusCode::NOT_FOUND:
      stream << "Not Found";
      break;
    case StatusCode::METHOD_NOT_ALLOWED:
      stream << "Method Not Allowed";
      break;
    case StatusCode::NOT_ACCEPTABLE:
      stream << "Not Acceptable";
      break;
    case StatusCode::PROXY_AUTHENTICATION_REQUIRED:
      stream << "Proxy Authentication Required";
      break;
    case StatusCode::REQUEST_TIME_OUT:
      stream << "Request Time-out";
      break;
    case StatusCode::CONFLICT:
      stream << "Conflict";
      break;
    case StatusCode::GONE:
      stream << "Gone";
      break;
    case StatusCode::LENGTH_REQUIRED:
      stream << "Length Required";
      break;
    case StatusCode::PRECONDITION_FAILED:
      stream << "Precondition Failed";
      break;
    case StatusCode::REQUEST_ENTITY_TOO_LARGE:
      stream << "Request Entity Too Large";
      break;
    case StatusCode::REQUEST_URI_TOO_LARGE:
      stream << "Request-URI Too Large";
      break;
    case StatusCode::UNSUPPORTED_MEDIA_TYPE:
      stream << "Unsupported Media Type";
      break;
    case StatusCode::REQUESTED_RANGE_NOT_SATISFIABLE:
      stream << "Requested range not satisfiable";
      break;
    case StatusCode::EXPECTATION_FAILED:
      stream << "Expectation Failed";
      break;
    case StatusCode::IM_A_TEAPOT:
      stream << "I'm a teapot (:";
      break;
    case StatusCode::AUTHENTICATION_TIMEOUT:
      stream << "Authentication Timeout";
      break;
    case StatusCode::METHOD_FAILURE:
      static_assert(
          StatusCode::METHOD_FAILURE == StatusCode::ENHANCE_YOUR_CALM,
          "Please, fix operator<< method"
      );
      stream << "Method Failure/Enhance Your Calm";
      break;
    case StatusCode::UNPROCESSABLE_ENTITY:
      stream << "Unprocessable Entity";
      break;
    case StatusCode::LOCKED:
      stream << "Locked";
      break;
    case StatusCode::FAILED_DEPENDENCY:
      stream << "Failed Dependency";
      break;
    case StatusCode::UNORDERED_COLLECTION:
      stream << "Unordered Collection";
      break;
    case StatusCode::UPGRADE_REQUIRED:
      stream << "Upgrade Required";
      break;
    case StatusCode::PRECONDITION_REQUIRED:
      stream << "Precondition Required";
      break;
    case StatusCode::TOO_MANY_REQUESTS:
      stream << "Too Many Requests";
      break;
    case StatusCode::REQUEST_HEADER_FIELDS_TOO_LARGE:
      stream << "Request Header Fields Too Large";
      break;
    case StatusCode::LOGIN_TIMEOUT:
      stream << "Login Timeout";
      break;
    case StatusCode::NO_RESPONSE:
      stream << "No response";
      break;
    case StatusCode::RETRY_WITH:
      stream << "Retry With";
      break;
    case StatusCode::BLOCKED_BY_WINDOWS_PARENTAL_CONTROLS:
      stream << "Blocked by Windows Parental Control";
      break;
    case StatusCode::UNAVAILABLE_FOR_LEGAL_REASONS:
      static_assert(
          StatusCode::UNAVAILABLE_FOR_LEGAL_REASONS == StatusCode::REDIRECT,
          "Please, fix operator<< method"
      );
      stream << "Unavailable For Legal Reasons/Redirect";
      break;
    case StatusCode::REQUEST_HEADER_TOO_LARGE:
      stream << "Request Header Too Large";
      break;
    case StatusCode::CERT_ERROR:
      stream << "Cert Error";
      break;
    case StatusCode::NO_CERT:
      stream << "No Cert";
      break;
    case StatusCode::HTTP_TO_HTTPS:
      stream << "HTTP to HTTPS";
      break;
    case StatusCode::CLIENT_CLOSED_REQUEST:
      stream << "Client Closed Request";
      break;
    case StatusCode::INTERNAL_SERVER_ERROR:
      stream << "Internal Server Error";
      break;
    case StatusCode::NOT_IMPLEMENTED:
      stream << "Not Implemented";
      break;
    case StatusCode::BAD_GATEWAY:
      stream << "Bad Gateway";
      break;
    case StatusCode::SERVICE_UNAVAILABLE:
      stream << "Service Unavailable";
      break;
    case StatusCode::GATEWAY_TIME_OUT:
      stream << "Gateway Time-out";
      break;
    case StatusCode::HTTP_VERSION_NOT_SUPPORTED:
      stream << "HTTP Version not supported";
      break;
    case StatusCode::VARIANT_ALSO_NEGOTIATES:
      stream << "Variant Also Negotiates";
      break;
    case StatusCode::INSUFFICIENT_STORAGE:
      stream << "Insufficient Storage";
      break;
    case StatusCode::LOOP_DETECTED:
      stream << "Loop Detected";
      break;
    case StatusCode::BANDWIDTH_LIMIT_EXCEEDED:
      stream << "Bandwidth Limit Exceeded";
      break;
    case StatusCode::NOT_EXTENDED:
      stream << "Not Extended";
      break;
    case StatusCode::NETWORK_AUTHENTICATION_REQUIRED:
      stream << "Network Authentication Required";
      break;
    case StatusCode::ORIGIN_ERROR:
      stream << "Origin Error";
      break;
    case StatusCode::WEB_SERVER_IS_DOWN:
      stream << "Web server is down";
      break;
    case StatusCode::CONNECTION_TIMED_OUT:
      stream << "Connection timed out";
      break;
    case StatusCode::PROXY_DECLINED_REQUEST:
      stream << "Proxy Declined Request";
      break;
    case StatusCode::A_TIMEOUT_OCCURRED:
      stream << "A timeout occurred";
      break;
    case StatusCode::NETWORK_READ_TIMEOUT_ERROR:
      stream << "Network read timeout error";
      break;
    case StatusCode::NETWORK_CONNECT_TIMEOUT_ERROR:
      stream << "Network connect timeout error";
      break;
#include <leathers/push>
#include <leathers/unreachable-code>
    default:
      stream << "!UNKNOWN!";
      break;
#include <leathers/pop>
  }
  stream << "(" << static_cast<int>(status_code) << ")";
  return stream;
}

} // namespace attribute
} // namespace response
} // namespace http
} // namespace network
} // namespace sober
