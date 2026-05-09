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
                                const tHeadersMap& requestHeaders,
                                const std::string& requestBody, 
                                tHeadersMap& responseHeaders,
                                RequestResponse& requestResponse) = 0;
};

}