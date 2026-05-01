#pragma once
#include <CHTTPRestController.h>
#include <CConfiguration.h>
#include <IKnocknockService.h>

namespace knocknock
{
class CHTTPChannel
{
public:
  CHTTPChannel(   IKnocknockService& knocknockService);
  ~CHTTPChannel();

  bool Initialize( const CConfiguration& config );
  void Shutdown();

private:
  IKnocknockService& m_knocknockService;
  HTTPServer::CHTTPRestController m_httpController;
};
  
} // namespace knocknock
