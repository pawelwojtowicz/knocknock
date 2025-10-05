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

const CSession& CSessionManager::Login(const tKeyValueMap& input, tKeyValueMap& output)
{
 
  std::string userId = {};
  std::string password = {};  
  auto newSession = m_sessionBuilder.CreateSession(input.at("userId"));

  if ( newSession && newSession->GetUserSessionState() == UserSessionState::CREATED )
  {
    auto& session = *newSession;

    auto sessionAuthenticationState = m_authenticator.Login(session, password);

  }

  return empty;
}

const CSession& CSessionManager::Authenticate(const tKeyValueMap& input, tKeyValueMap& output)
{
  return empty;
}

const bool CSessionManager::Logout(const tKeyValueMap& input, tKeyValueMap& output)
{
  return false;
}

const bool CSessionManager::Touch(const tKeyValueMap& input, tKeyValueMap& output)
{
  return false;  
}

}