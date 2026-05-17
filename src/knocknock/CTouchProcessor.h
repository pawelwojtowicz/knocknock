#pragma once
#include <IRequestProcessor.h>
#include <IKnocknockService.h>

namespace knocknock
{
class CCookieBuilder;

class CTouchProcessor : public HTTPServer::IRequestProcessor
{
public:
  CTouchProcessor(IKnocknockService& knocknockService, CCookieBuilder& cookieBuilder);
  ~CTouchProcessor() override = default;

  bool ProcessRequest(  const HTTPServer::URLInfo& urlInfo,
                        const HTTPServer::tHeadersMap& requestHeaders,
                        const std::string& requestBody,
                        HTTPServer::tHeadersMap& responseHeaders,
                        HTTPServer::RequestResponse& requestResponse ) override;

private:
  IKnocknockService& m_knocknockService;
  CCookieBuilder& m_cookieBuilder;
};

} // namespace knocknock