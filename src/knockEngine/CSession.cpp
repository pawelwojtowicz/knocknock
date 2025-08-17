#include "CSession.h"

namespace knocknock
{

CSession::CSession(const std::string& sessionId, const std::string& userId, const std::string& userName, const std::string& authMethod, const std::string& authString) 
: m_sid(sessionId)
, m_userId(userId)
, m_userName(userName)
, m_authMethod(authMethod)
, m_authString(authString)
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

int CSession::GetCookieMaxAge() const
{
  return m_cookieMaxAge;
}

const std::string& CSession::GetCookieExpires() const
{
  return m_cookieExpires;
}

bool CSession::GetCookieHttpOnly() const
{
  return m_cookieHttpOnly;
}

const std::string& CSession::GetCookiePath() const
{
  return m_cookiePath;
}

bool CSession::GetCookieSecure() const
{
  return m_cookieSecure;
}

CookieSameSite CSession::GetCookieSameSite() const
{
  return m_cookieSameSite;
}

}