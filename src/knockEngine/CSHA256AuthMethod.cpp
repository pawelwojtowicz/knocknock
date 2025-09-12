#include "CSHA256AuthMethod.h"
#include "CSession.h"
#include <CSHA256Hash.h>

namespace knocknock {

std::string CSHA256AuthMethod::Login(CSession& session, const std::string& username, const std::string& password)
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
    return "Invalid username or password";
  } 

  session.UpdateUserSessionState(UserSessionState::READY);
  return {};
}

std::string CSHA256AuthMethod::Authenticate(CSession& session, const std::string& authenticationPayload)
{
  //method does not require the 2nd step. Session is immediately authenticated after creation
  return {};
}

} // namespace knocknock