#pragma once
#include <tuple>
#include <list>
#include <memory>
#include "HTTPServerTypes.h"
#include "IServiceContext.h"
#include "IRequestProcessor.h"
#include "IURLMatcher.h"

namespace HTTPServer
{
class CProcessorRegistry : public IServiceContext
{
  using tProcessorRecord = std::tuple<HttpMethod,std::unique_ptr<IURLMatcher>, std::shared_ptr<IRequestProcessor>>;
  using tProcessorList = std::list<tProcessorRecord>;
public:
  CProcessorRegistry() = default;
  virtual ~CProcessorRegistry() = default;

  bool RegisterRequestProcessor( const HTTPServer::HttpMethod method, const std::string& urlPattern, std::shared_ptr<IRequestProcessor>& processor );

private:
  bool ProcessRequest(  const HTTPServer::HttpMethod method, 
                        const std::string& url, 
                        const std::string& requestBody, 
                        std::string& responseBody) override;

private:
  tProcessorList m_processors;
};
}