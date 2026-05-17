#pragma once
#include <IRequestProcessor.h>
#include <IKnocknockService.h>

namespace knocknock
{
class CCookieBuilder;

class CLogoutProcessor : public HTTPServer::IRequestProcessor
{
public:
  CLogoutProcessor(IKnocknockService& knocknockService, CCookieBuilder& cookieBuilder);
  ~CLogoutProcessor() override = default;

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