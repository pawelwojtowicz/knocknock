#pragma once
#include <IRequestProcessor.h>
#include <IKnocknockService.h>

namespace knocknock
{
class CLoginProcessor : public HTTPServer::IRequestProcessor
{
public:
  CLoginProcessor(IKnocknockService& knocknockService);
  ~CLoginProcessor() override = default;

  bool ProcessRequest(  const HTTPServer::URLInfo& urlInfo,
                        const HTTPServer::tHeadersMap& requestHeaders,
                        const std::string& requestBody,
                        HTTPServer::tHeadersMap& responseHeaders,
                        std::string& responseBody ) override;

private:
  IKnocknockService& m_knocknockService;
};

} // namespace knocknock  