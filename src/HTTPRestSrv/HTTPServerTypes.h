#pragma once
#include <string>

namespace HTTPServer
{
enum class HttpMethod 
{
  mthd_POST,
  mthd_GET,
  mthd_PUT,
  mthd_PATCH,
  mthd_DELETE,
  mthd_INVALID
};

}