#pragma once
#include <IRequestProcessor.h>
#include <IKnocknockService.h>

namespace knocknock
{
class CCookieBuilder;

class CAuthProcessor : public HTTPServer::IRequestProcessor
{
public:
  CAuthProcessor(IKnocknockService& knocknockService, CCookieBuilder& cookieBuilder);
  ~CAuthProcessor() override = default;

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