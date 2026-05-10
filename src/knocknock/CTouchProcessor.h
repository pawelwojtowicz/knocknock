#pragma once
#include <IRequestProcessor.h>
#include <IKnocknockService.h>

namespace knocknock
{
class CTouchProcessor : public HTTPServer::IRequestProcessor
{
public:
  CTouchProcessor(IKnocknockService& knocknockService);
  ~CTouchProcessor() override = default;

  bool ProcessRequest(  const HTTPServer::URLInfo& urlInfo,
                        const HTTPServer::tHeadersMap& requestHeaders,
                        const std::string& requestBody,
                        HTTPServer::tHeadersMap& responseHeaders,
                        HTTPServer::RequestResponse& requestResponse ) override;

private:
  IKnocknockService& m_knocknockService;
};

} // namespace knocknock