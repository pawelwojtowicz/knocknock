#include "CSimpleDBAuthMethod.h"
#include "CSession.h"

namespace knocknock {

std::string CSimpleDBAuthMethod::Login(CSession& session, const std::string& username, const std::string& password)
{
  session.UpdateUserSessionState(UserSessionState::READY);
  return {};
}

std::string CSimpleDBAuthMethod::Authenticate(CSession& session, const std::string& authenticationPayload)
{
  //method does not require the 2nd step. Session is immediately authenticated after creation
  return {};
}

} // namespace knocknock