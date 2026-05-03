#pragma once
#include <map>
#include <string>
#include "HTTPServerTypes.h"

namespace HTTPServer
{

class IServiceContext
{
public:
  IServiceContext() = default;
  virtual ~IServiceContext() = default;

  virtual bool ProcessRequest(  const HTTPServer::HttpMethod method, 
                                const std::string& url, 
                                const std::map<std::string, std::string>& requestHeaders,
                                const std::string& requestBody, 
                                std::map<std::string, std::string>& responseHeaders,
                                std::string& responseBody) = 0;
};

}