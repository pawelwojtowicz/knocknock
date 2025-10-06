#include "CSessionManager.h"
#include "CKeyValueHelper.h"
#include "KnocKnockDictionary.h"

namespace knocknock
{

CSessionManager::CSessionManager( DBAccess::IDBAccess& rDBAccess, CConfiguration& rConfiguration )
: m_rDBAccess(rDBAccess)
, m_rConfiguration(rConfiguration)
, m_sessionBuilder(rConfiguration, rDBAccess)
, m_authenticator()
, m_emptySession("", "", "", "", "")
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
  CKeyValueHelper inputHelper(input);

  std::string userId = {};
  if (inputHelper.GetValue(sLoginUserId, userId))
  {
    auto newSession = m_sessionBuilder.CreateSession(userId);
    if ( newSession && newSession->GetUserSessionState() == UserSessionState::CREATED )
    {
      auto& session = *newSession;
      auto sessionAuthenticationState = m_authenticator.Login(session, input);

      return session;
    }

  }  
  return m_emptySession;
}

const CSession& CSessionManager::Authenticate(const tKeyValueMap& input, tKeyValueMap& output)
{
  return m_emptySession;
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