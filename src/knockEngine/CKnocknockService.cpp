#include "CKnocknockService.h"

namespace knocknock
{

CKnocknockService::CKnocknockService( DBAccess::IDBAccess& rDBAccess, CConfiguration& rConfiguration )
: m_rDBAccess(rDBAccess)
, m_rConfiguration(rConfiguration)
, m_sessionBuilder(rConfiguration, rDBAccess)
, m_authenticator()
{

}

CKnocknockService::~CKnocknockService()
{

}

bool CKnocknockService::Initialize()
{
  m_authenticator.Initialize(m_rConfiguration);
  return false;
}

void CKnocknockService::Shutdown()
{
  m_authenticator.Shutdown();
}

const tKeyValueMap CKnocknockService::Login(const std::string& userId, const std::string& password)
{
  return {};
}

const tKeyValueMap CKnocknockService::Authenticate(const std::string& sessionId, const tKeyValueMap& authenticationPayload)
{
  return {};
}

const tKeyValueMap CKnocknockService::Logout(const std::string& sessionId)
{
  return {};
}

const tKeyValueMap CKnocknockService::Touch(const std::string& sessionId)
{
  return {};
}

}