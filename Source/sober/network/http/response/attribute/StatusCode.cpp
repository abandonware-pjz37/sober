// Copyright (c) 2014, Ruslan Baratov
// All rights reserved.

#include <sober/network/http/response/attribute/StatusCode.hpp>

#include <iostream> // std::ostream

namespace sober {
namespace network {
namespace http {
namespace response {
namespace attribute {

std::ostream& operator<< (std::ostream& stream, StatusCode status_code) {
  switch (status_code) {
    case StatusCode::CONTINUE:
      stream << "Continue (100)";
      break;
    case StatusCode::SWITCHING_PROTOCOLS:
      stream << "Switching Protocols (101)";
      break;
    case StatusCode::OK:
      stream << "OK (200)";
      break;
    case StatusCode::CREATED:
      stream << "Created (201)";
      break;
    case StatusCode::ACCEPTED:
      stream << "Accepted (202)";
      break;
    case StatusCode::NON_AUTHORITATIVE_INFORMATION:
      stream << "Non-Authoritative Information (203)";
      break;
    case StatusCode::NO_CONTENT:
      stream << "No Content (204)";
      break;
    case StatusCode::RESET_CONTENT:
      stream << "Reset Content (205)";
      break;
    case StatusCode::PARTIAL_CONTENT:
      stream << "Partial Content (206)";
      break;
    case StatusCode::MULTIPLE_CHOICES:
      stream << "Multiple Choices (300)";
      break;
    case StatusCode::MOVED_PERMANENTLY:
      stream << "Moved Permanently (301)";
      break;
    case StatusCode::FOUND:
      stream << "Found (302)";
      break;
    case StatusCode::SEE_OTHER:
      stream << "See Other (303)";
      break;
    case StatusCode::NOT_MODIFIED:
      stream << "Not Modified (304)";
      break;
    case StatusCode::USE_PROXY:
      stream << "Use Proxy (305)";
      break;
    case StatusCode::TEMPORARY_REDIRECT:
      stream << "Temporary Redirect (307)";
      break;
    case StatusCode::BAD_REQUEST:
      stream << "Bad Request (400)";
      break;
    case StatusCode::UNAUTHORIZED:
      stream << "Unauthorized (401)";
      break;
    case StatusCode::PAYMENT_REQUIRED:
      stream << "Payment Required (402)";
      break;
    case StatusCode::FORBIDDEN:
      stream << "Forbidden (403)";
      break;
    case StatusCode::NOT_FOUND:
      stream << "Not Found (404)";
      break;
    case StatusCode::METHOD_NOT_ALLOWED:
      stream << "Method Not Allowed (405)";
      break;
    case StatusCode::NOT_ACCEPTABLE:
      stream << "Not Acceptable (406)";
      break;
    case StatusCode::PROXY_AUTHENTICATION_REQUIRED:
      stream << "Proxy Authentication Required (407)";
      break;
    case StatusCode::REQUEST_TIME_OUT:
      stream << "Request Time-out (408)";
      break;
    case StatusCode::CONFLICT:
      stream << "Conflict (409)";
      break;
    case StatusCode::GONE:
      stream << "Gone (410)";
      break;
    case StatusCode::LENGTH_REQUIRED:
      stream << "Length Required (411)";
      break;
    case StatusCode::PRECONDITION_FAILED:
      stream << "Precondition Failed (412)";
      break;
    case StatusCode::REQUEST_ENTITY_TOO_LARGE:
      stream << "Request Entity Too Large (413)";
      break;
    case StatusCode::REQUEST_URI_TOO_LARGE:
      stream << "Request-URI Too Large (414)";
      break;
    case StatusCode::UNSUPPORTED_MEDIA_TYPE:
      stream << "Unsupported Media Type (415)";
      break;
    case StatusCode::REQUESTED_RANGE_NOT_SATISFIABLE:
      stream << "Requested range not satisfiable (416)";
      break;
    case StatusCode::EXPECTATION_FAILED:
      stream << "Expectation Failed (417)";
      break;
    case StatusCode::INTERNAL_SERVER_ERROR:
      stream << "Internal Server Error (500)";
      break;
    case StatusCode::NOT_IMPLEMENTED:
      stream << "Not Implemented (501)";
      break;
    case StatusCode::BAD_GATEWAY:
      stream << "Bad Gateway (502)";
      break;
    case StatusCode::SERVICE_UNAVAILABLE:
      stream << "Service Unavailable (503)";
      break;
    case StatusCode::GATEWAY_TIME_OUT:
      stream << "Gateway Time-out (504)";
      break;
    case StatusCode::HTTP_VERSION_NOT_SUPPORTED:
      stream << "HTTP Version not supported (505)";
      break;
    default:
      stream << "!UNKNOWN! (" << static_cast<int>(status_code) << ")";
      break;
  }
  return stream;
}

} // namespace attribute
} // namespace response
} // namespace http
} // namespace network
} // namespace sober
