#pragma once
#include <string>
#include <cstdint>
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

  void AddAuthenticationStateVariable( const std::string& key, const std::string& value ) { m_authenticationState.insert_or_assign(key, value); }
  const std::string GetAuthenticationStateVariable( const std::string& key ) const;

  void SetMaxAge(int64_t maxAge) { m_sessionMaxAge = maxAge; }
  int64_t GetMaxAge() const { return m_sessionMaxAge; }

  void SetSessionExpires(int64_t expires) { m_sessionExpires = expires; }
  int64_t GetSessionExpires() const { return m_sessionExpires; }


private:
  std::string m_sid;
  std::string m_userId;
  std::string m_userName;
  std::string m_authMethod;
  std::string m_authString;
  int64_t m_sessionMaxAge;
  int64_t m_sessionExpires;

  UserSessionState m_userSessionState;

  tKeyValueMap m_authenticationState;
};

}