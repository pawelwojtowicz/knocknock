#include "CSessionManager.h"
#include "CKeyValueHelper.h"
#include "KnocKnockDictionary.h"
#include "CTimespan.h"

namespace knocknock
{

CSessionManager::CSessionManager( DBAccess::IDBAccess& rDBAccess, CConfiguration& rConfiguration )
: m_rDBAccess(rDBAccess)
, m_rConfiguration(rConfiguration)
, m_sessionBuilder(rConfiguration, rDBAccess)
, m_authenticator()
, m_emptySession("", "", "", "", "")
, m_sessionsMutex()
, m_sessions()
, m_sessionExpirationTimeout{180} // default to 3 minutes, can be overridden by configuration
{

}

CSessionManager::~CSessionManager()
{

}

bool CSessionManager::Initialize()
{
  int m_sessionExpirationTimeout = m_rConfiguration.GetParamInt( "sessionExpirationTimeout", m_sessionExpirationTimeout);

  if ( !m_sessionBuilder.Initialize() )
  {
    return false;
  } 
  
  if ( !m_authenticator.Initialize(m_rConfiguration) )
  {
    return false;
  }

  return true;
}

void CSessionManager::Shutdown()
{
  m_authenticator.Shutdown();
}

const CSession CSessionManager::Login(const tKeyValueMap& input, tKeyValueMap& output)
{
  CKeyValueHelper inputHelper(input);

  std::string userId = {};
  if (inputHelper.GetValue(sLoginUserId, userId))
  {
    auto newSession = m_sessionBuilder.CreateSession(userId);
    if ( newSession && newSession->GetUserSessionState() == UserSessionState::CREATED )
    {
      auto& session = *newSession;
      output = m_authenticator.Login(session, input);
      if ( session.GetUserSessionState() == UserSessionState::AUTH_SUCCESS )
      {
        session.SetSessionExpires(CTimespan::GetEpochSeconds() + m_sessionExpirationTimeout);
      }

      {
        std::lock_guard<std::shared_mutex> lock(m_sessionsMutex);
        m_sessions.insert({session.GetSessionId(), session});
      }

      return session;
    }

  }  
  return m_emptySession;
}

const CSession CSessionManager::Authenticate(const tKeyValueMap& input, tKeyValueMap& output)
{
  CKeyValueHelper inputHelper(input);
  std::string sessionId{};
  if ( inputHelper.GetValue(sLoginSessionId, sessionId) && !sessionId.empty() )
  {
    std::lock_guard<std::shared_mutex> lock(m_sessionsMutex);
    auto sessionIt = m_sessions.find(sessionId);
    if ( m_sessions.end() != sessionIt  )
    {
      CSession& session = sessionIt->second;
      if ( UserSessionState::AUTH_IN_PROGRESS == session.GetUserSessionState() && session.GetSessionExpires() > CTimespan::GetEpochSeconds() )
      {
        output = m_authenticator.Authenticate(session, input);
        if ( session.GetUserSessionState() == UserSessionState::AUTH_SUCCESS )
        {
          session.SetSessionExpires(CTimespan::GetEpochSeconds() + m_sessionExpirationTimeout);
        }
      }
      return session;
    }
  } 
  return m_emptySession;
}

const bool CSessionManager::Logout(const tKeyValueMap& input, tKeyValueMap& output)
{
  CKeyValueHelper inputHelper(input);
  std::string sessionId{};
  if ( inputHelper.GetValue(sLoginSessionId, sessionId) && !sessionId.empty() )
  {
    std::lock_guard<std::shared_mutex> lock(m_sessionsMutex);
    auto sessionIt = m_sessions.find(sessionId);
    if ( sessionIt != m_sessions.end() )
    {
      CSession& session = sessionIt->second;
      session.UpdateUserSessionState(UserSessionState::LOGGED_OUT);
      return true;
    }
  } 
  return false;
}

const CSession CSessionManager::Touch(const tKeyValueMap& input, tKeyValueMap& output)
{
  CKeyValueHelper inputHelper(input);
  std::string sessionId{};
  int currentTime = CTimespan::GetEpochSeconds();
  if ( inputHelper.GetValue(sLoginSessionId, sessionId) && !sessionId.empty() )
  {
    std::lock_guard<std::shared_mutex> lock(m_sessionsMutex);
    auto sessionIt = m_sessions.find(sessionId);
    if ( sessionIt != m_sessions.end() )
    {
      CSession& session = sessionIt->second;
      
      if ( UserSessionState::AUTH_SUCCESS == session.GetUserSessionState() && session.GetSessionExpires() > currentTime )
      {
        session.SetSessionExpires(currentTime + m_sessionExpirationTimeout);
        return session;
      }
    }
  } 
  return m_emptySession;
}

}