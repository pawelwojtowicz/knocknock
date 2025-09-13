#include "CSimpleDBAuthMethod.h"
#include "CSession.h"

namespace knocknock {

tKeyValueMap CSimpleDBAuthMethod::Login(CSession& session, const std::string& password)
{
  session.UpdateUserSessionState(UserSessionState::READY);
  return {};
}

tKeyValueMap CSimpleDBAuthMethod::Authenticate(CSession& session, const tKeyValueMap& authenticationPayload)
{
  //method does not require the 2nd step. Session is immediately authenticated after creation
  return {};
}

} // namespace knocknock