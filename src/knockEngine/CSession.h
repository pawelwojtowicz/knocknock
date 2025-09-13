#pragma once
#include <string>
#include "knocknockTypes.h"

namespace knocknock
{

class CSession
{
public:
  CSession(const std::string& sessionId, const std::string& userId, const std::string& userName, const std::string& authMethod, const std::string& authString);
  virtual ~CSession() = default;

  const std::string& GetSessionId() const;
  const std::string& GetUserId() const;
  const std::string& GetUserName() const;
  const std::string& GetAuthMethod() const { return m_authMethod; }
  const std::string& GetAuthString() const { return m_authString; }

  void UpdateUserSessionState(UserSessionState newState) { m_userSessionState = newState; }
  UserSessionState GetUserSessionState() const { return m_userSessionState; }

  void AddAuthenticationStateVariable( const std::string& key, const std::string& value ) { m_authenticationState[key] = value; }
  const std::string GetAuthenticationStateVariable( const std::string& key ) const 
  { 
    const auto it = m_authenticationState.find(key);
    if (it != m_authenticationState.end())
    {
      return it->second;
    }
    return "";
  }


private:
  std::string m_sid;
  std::string m_userId;
  std::string m_userName;
  std::string m_authMethod;
  std::string m_authString;
  int m_sessionMaxAge;
  int m_sessionExpires;

  UserSessionState m_userSessionState;

  tKeyValueMap m_authenticationState;
};

}