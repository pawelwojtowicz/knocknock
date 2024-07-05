#pragma once
#include "CUser.h"

namespace knocknock
{

class CUserSession
{
public:
  CUserSession() = default;
  virtual ~CUserSession() = default;

private:
  std::string m_sessionId;

  std::string m_authenticationChallenge;

  std::string m_cryptoIV;

  CUser m_userRecord;
};

}