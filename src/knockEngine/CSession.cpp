#include "CSession.h"

namespace knocknock
{
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