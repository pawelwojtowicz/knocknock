#pragma once
#include "HTTPServerTypes.h"

namespace HTTPServer
{

class IServiceContext
{
public:
  IServiceContext() = default;
  virtual ~IServiceContext() = default;

  virtual bool ProcessRequest( const HTTPServer::HttpMethod method, const std::string& url, const std::string& requestBody, std::string& responseBody) = 0;
};

}