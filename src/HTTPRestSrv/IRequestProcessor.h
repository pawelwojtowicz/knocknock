#pragma once
#include <map>
#include <string>

namespace HTTPServer
{
struct URLInfo;

using tHeadersMap = std::map<std::string, std::string>;

class IRequestProcessor
{
public:
  IRequestProcessor() = default;
  virtual ~IRequestProcessor() = default;

  virtual bool ProcessRequest(  const URLInfo& urlInfo, 
                                const tHeadersMap& requestHeaders, 
                                const std::string& requestBody, 
                                tHeadersMap& responseHeaders,
                                std::string& responseBody ) = 0;
};
}