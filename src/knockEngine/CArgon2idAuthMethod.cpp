#include "CArgon2idAuthMethod.h"
#include "CSession.h"
#include <CArgon2idWrapper.h>
#include "CKeyValueHelper.h"

namespace knocknock {

tKeyValueMap CArgon2idAuthMethod::Login(CSession& session, const tKeyValueMap& loginPayload)
{
  std::string password = {};

  CKeyValueHelper loginPayloadHelper(loginPayload);

  if (!loginPayloadHelper.GetValue(sLoginPassword, password))
  {
    return { tKeyValueMap::value_type("error", "missing_password") };
  }

  // The authString stored in the DB is the Argon2id encoded hash
  const std::string& expectedHash = session.GetAuthString();
  if (expectedHash.empty())
  {
    session.UpdateUserSessionState(UserSessionState::AUTH_FAILED);
    return { tKeyValueMap::value_type("error", "missing_auth_string") };
  }

  if (!CryptoTools::CArgon2idWrapper::VerifyPassword(password, expectedHash))
  {
    session.UpdateUserSessionState(UserSessionState::AUTH_FAILED);
    return { tKeyValueMap::value_type("error", "invalid_credentials") };
  }

  session.UpdateUserSessionState(UserSessionState::AUTH_SUCCESS);
  return {};
}

tKeyValueMap CArgon2idAuthMethod::Authenticate(CSession& session, const tKeyValueMap& authenticationPayload)
{
  // Method does not require a 2nd step. Session is immediately authenticated after Login.
  return {};
}

} // namespace knocknock
