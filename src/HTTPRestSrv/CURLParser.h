#pragma once
#include "URLInfo.h"
#include <string>

namespace HTTPServer
{

class CURLParser
{
public:
  static bool Parse( const std::string& url, URLInfo& oUrlInfo);
};

}