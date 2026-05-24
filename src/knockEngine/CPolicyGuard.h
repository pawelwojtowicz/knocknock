#pragma once
#include "knocknockTypes.h"
#include "CSession.h"
#include <IDBAccess.h>

namespace knocknock
{

class CPolicyGuard
{
public:
  CPolicyGuard( tSessionMap& sessionMap );
  ~CPolicyGuard() = default;

  bool Initialize(DBAccess::IDBAccess& dbAccess);
  void Shutdown();

  bool VerifySession( CSession& session );

private: 
  tSessionMap& m_rSessionMap;
};
}