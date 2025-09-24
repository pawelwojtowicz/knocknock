#include "CSHA256AuthMethod.h"
#include "CSession.h"
#include <CSHA256Hash.h>

namespace knocknock {

tKeyValueMap CSHA256AuthMethod::Login(CSession& session, const std::string& password)
{
  std::string passwordHashRaw{};
  std::string passwordHashString{};
  if (!CSHA256Hash::CalculateHash(password, passwordHashRaw, passwordHashString))
  {
    return {};
  }

  if ( session.GetAuthString() != passwordHashString)
  {
    session.UpdateUserSessionState(UserSessionState::AUTH_FAILED);
    return { tKeyValueMap::value_type("error", "invalid_credentials") };
  } 

  session.UpdateUserSessionState(UserSessionState::AUTH_SUCCESS);
  return {};
}

tKeyValueMap CSHA256AuthMethod::Authenticate(CSession& session, const tKeyValueMap& authenticationPayload)
{
  //method does not require the 2nd step. Session is immediately authenticated after creation
  return {};
}

} // namespace knocknock