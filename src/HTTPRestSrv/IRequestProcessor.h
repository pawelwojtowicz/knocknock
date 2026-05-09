#pragma once
#include <map>
#include <string>
#include "HTTPServerTypes.h"

namespace HTTPServer
{
struct URLInfo;

class IRequestProcessor
{
public:
  IRequestProcessor() = default;
  virtual ~IRequestProcessor() = default;

  virtual bool ProcessRequest(  const URLInfo& urlInfo, 
                                const tHeadersMap& requestHeaders, 
                                const std::string& requestBody, 
                                tHeadersMap& responseHeaders,
                                RequestResponse& requestResponse ) = 0;
};
}