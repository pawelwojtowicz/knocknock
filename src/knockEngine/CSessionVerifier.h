#pragma once
#include "knocknockTypes.h"
#include "CSession.h"

namespace knocknock
{ 
class CSessionVerifier
{
public:
  CSessionVerifier() = default;
  virtual ~CSessionVerifier() = default;

  bool VerifySession(const CSession& session) const;
};
}