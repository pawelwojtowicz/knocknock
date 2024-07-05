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

  CUser m_userRecord;

};

}