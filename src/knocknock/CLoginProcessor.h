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

  bool ProcessRequest( const HTTPServer::URLInfo& urlInfo, const std::string& requestBody, std::string& responseBody ) override;

private:
  IKnocknockService& knocknockService;
};

} // namespace knocknock  