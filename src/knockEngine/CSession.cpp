#include "CSession.h"

namespace knocknock
{

CSession::CSession(const std::string& sessionId, const std::string& userId, const std::string& userName, const std::string& authMethod, const std::string& authString) 
: m_sid(sessionId)
, m_userId(userId)
, m_userName(userName)
, m_authMethod(authMethod)
, m_authString(authString)
, m_userSessionState(UserSessionState::CREATED)
{
}

const std::string& CSession::GetSessionId() const
{
  return m_sid;
}

const std::string& CSession::GetUserId() const
{
  return m_userId;
}

const std::string& CSession::GetUserName() const
{
  return m_userName;
}

const std::string CSession::GetAuthenticationStateVariable( const std::string& key ) const 
{ 
  const auto it = m_authenticationState.find(key);
  if (it != m_authenticationState.end())
  {
    return it->second;
  }
  return "";
}


}