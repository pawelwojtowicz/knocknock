#pragma once
#include <string>

namespace HTTPServer
{
struct URLInfo;

class IRequestProcessor
{
public:
  IRequestProcessor() = default;
  virtual ~IRequestProcessor() = default;

  virtual bool ProcessRequest( const URLInfo& urlInfo, const std::string& requestBody, std::string& responseBody ) = 0;
};
}