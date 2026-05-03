#pragma once
#include <map>
#include <string>

namespace HTTPServer
{
struct URLInfo;

class IRequestProcessor
{
public:
  IRequestProcessor() = default;
  virtual ~IRequestProcessor() = default;

  virtual bool ProcessRequest(  const URLInfo& urlInfo, 
                                const std::map<std::string, std::string>& requestHeaders, 
                                const std::string& requestBody, 
                                std::map<std::string, std::string>& responseHeaders,
                                std::string& responseBody ) = 0;
};
}