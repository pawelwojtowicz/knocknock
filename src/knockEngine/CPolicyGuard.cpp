#include "CPolicyGuard.h"

namespace knocknock
{

CPolicyGuard::CPolicyGuard( tSessionMap& sessionMap )
: m_rSessionMap(sessionMap)
{
} 

bool CPolicyGuard::Initialize(DBAccess::IDBAccess& dbAccess)
{
  return true;
}
  
void CPolicyGuard::Shutdown()
{

}

bool CPolicyGuard::VerifySession( CSession& session)
{
  //temporarily - before we implement the actual policy checks, we will just mark the session as valid
  if (session.GetUserSessionState() == UserSessionState::AUTH_SUCCESS)
  {
    session.UpdateUserSessionState(UserSessionState::VALID);
  }
  return true;
}

}