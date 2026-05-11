#pragma once
#include <string>
#include <map>

namespace HTTPServer
{

struct RequestResponse
{
std::string responseBody = {};
int responseCode = { -1 };
};

enum class HttpMethod 
{
  mthd_POST,
  mthd_GET,
  mthd_PUT,
  mthd_PATCH,
  mthd_DELETE,
  mthd_INVALID
};

using tHeadersMap = std::map<std::string, std::string>;
}