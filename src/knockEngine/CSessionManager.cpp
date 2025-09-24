#include "CSessionManager.h"

namespace knocknock
{

CSessionManager::CSessionManager( DBAccess::IDBAccess& rDBAccess, CConfiguration& rConfiguration )
: m_rDBAccess(rDBAccess)
, m_rConfiguration(rConfiguration)
, m_sessionBuilder(rConfiguration, rDBAccess)
, m_authenticator()
{

}

CSessionManager::~CSessionManager()
{

}

bool CSessionManager::Initialize()
{
  m_sessionBuilder.Initialize();
  m_authenticator.Initialize(m_rConfiguration);
  return false;
}

void CSessionManager::Shutdown()
{
  m_authenticator.Shutdown();
}

const tKeyValueMap CSessionManager::Login(const std::string& userId, const std::string& password)
{
  return {};
}

const tKeyValueMap CSessionManager::Authenticate(const std::string& sessionId, const tKeyValueMap& authenticationPayload)
{
  return {};
}

const tKeyValueMap CSessionManager::Logout(const std::string& sessionId)
{
  return {};
}

const tKeyValueMap CSessionManager::Touch(const std::string& sessionId)
{
  return {};
}

}