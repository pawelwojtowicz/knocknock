#include "CHTTPChannel.h"
#include <HTTPServerConfig.h>
#include "CLoginProcessor.h"
#include "CAuthProcessor.h"
#include "CTouchProcessor.h"
#include "CLogoutProcessor.h"
#include <memory>

namespace knocknock
{
CHTTPChannel::CHTTPChannel( IKnocknockService& knocknockService )
: m_knocknockService( knocknockService )
, m_httpController()
{
}

CHTTPChannel::~CHTTPChannel()
{
}

bool CHTTPChannel::Initialize( const CConfiguration& config )
{
  HTTPServer::HTTPServerConfig httpConfig;
  httpConfig.port = config.GetParamInt("httpServer.port", 8080);
  httpConfig.threadPoolSize = config.GetParamInt("httpServer.threadPoolSize", 4);

  m_httpController.RegisterProcessor( HTTPServer::HttpMethod::mthd_POST, "/login", std::make_shared<CLoginProcessor>(m_knocknockService) );
  m_httpController.RegisterProcessor( HTTPServer::HttpMethod::mthd_POST, "/auth", std::make_shared<CAuthProcessor>(m_knocknockService) );
  m_httpController.RegisterProcessor( HTTPServer::HttpMethod::mthd_POST, "/touch", std::make_shared<CTouchProcessor>(m_knocknockService) );
  m_httpController.RegisterProcessor( HTTPServer::HttpMethod::mthd_POST, "/logout", std::make_shared<CLogoutProcessor>(m_knocknockService) );

  return m_httpController.Initialize( httpConfig );
}

void CHTTPChannel::Shutdown()
{
  m_httpController.Shutdown();
}

} // namespace knocknock