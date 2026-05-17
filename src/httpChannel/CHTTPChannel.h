#pragma once
#include <CConfiguration.h>
#include "CCookieBuilder.h"
#include <IKnocknockService.h>
#include <CHTTPRestController.h>

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
  CCookieBuilder m_cookieBuilder;
  IKnocknockService& m_knocknockService;
  HTTPServer::CHTTPRestController m_httpController;
};
  
} // namespace knocknock