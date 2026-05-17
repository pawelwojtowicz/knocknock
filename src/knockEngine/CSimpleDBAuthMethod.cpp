#include "CSimpleDBAuthMethod.h"
#include "CSession.h"

namespace knocknock {

tKeyValueMap CSimpleDBAuthMethod::Login(CSession& session, const tKeyValueMap& loginPayload)
{
  // The authentication succeeds if the user's record can be found in the DB. It is sufficient
  // to consider the user as succesfully authenticated.
  session.UpdateUserSessionState(UserSessionState::AUTH_SUCCESS);
  return {};
}

tKeyValueMap CSimpleDBAuthMethod::Authenticate(CSession& session, const tKeyValueMap& authenticationPayload)
{
  //method does not require the 2nd step. Session is immediately authenticated after creation
  return {};
}

} // namespace knocknock