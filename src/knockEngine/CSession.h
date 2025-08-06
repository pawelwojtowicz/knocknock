#pragma once
#include <string>
#include "knocknockTypes.h"

namespace knocknock
{

class CSession
{
public:
  CSession() = default;
  virtual ~CSession() = default;

  const std::string& GetSessionId() const;

  const std::string& GetUserId() const;

  const std::string& GetUserName() const;

  int GetCookieMaxAge() const;
  const std::string& GetCookieExpires() const;
  bool GetCookieHttpOnly() const;
  const std::string& GetCookiePath() const;
  bool GetCookieSecure() const;
  CookieSameSite GetCookieSameSite() const;

private:
  std::string m_sid;
  std::string m_userId;
  std::string m_userName;

  int m_cookieMaxAge;
  std::string m_cookieExpires;
  bool m_cookieHttpOnly;
  std::string m_cookiePath;
  bool m_cookieSecure;
  CookieSameSite m_cookieSameSite;


};

}