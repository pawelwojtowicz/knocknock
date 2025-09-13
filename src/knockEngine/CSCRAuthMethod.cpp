#include "CSCRAuthMethod.h"
#include "CSession.h"
#include <CSHA256Hash.h>
#include <CAuthenticationChallengeTools.h>
#include <CAESCipherWrapper.h>

namespace knocknock {

constexpr const char* SCR_KEY_CHALLENGE = "challenge";
constexpr const char* SCR_KEY_RESPONSE = "challenge_response";

tKeyValueMap CSCRAuthMethod::Login(CSession& session, const std::string&)
{
  tKeyValueMap response{};
  const std::string challenge = CAuthenticationChallengeTools::GenerateAuthenticationChallenge(session.GetUserId());

  response[SCR_KEY_CHALLENGE] = challenge;
  session.AddAuthenticationStateVariable(SCR_KEY_CHALLENGE, challenge);
  session.UpdateUserSessionState(UserSessionState::AUTHENTICATING);

  return response;
}

tKeyValueMap CSCRAuthMethod::Authenticate(CSession& session, const tKeyValueMap& authenticationPayload)
{
  tKeyValueMap response{};
  
  if (session.GetUserSessionState() != UserSessionState::AUTHENTICATING)
  {
    response["error"] = "invalid_session_state";
    return response;
  }

  // assume pessimistic outcome
  session.UpdateUserSessionState(UserSessionState::AUTH_FAILED);

  const auto it = authenticationPayload.find(SCR_KEY_RESPONSE);
  if (it == authenticationPayload.end())
  {
    response["error"] = "missing_response";
    return response;
  }

  const std::string& challengeResponse  = it->second;
  const std::string authChallenge = session.GetAuthenticationStateVariable(SCR_KEY_CHALLENGE);
  if (authChallenge.empty())
  {
    response["error"] = "missing_challenge_in_session";
    return response;
  }

  std::string expectedResponse{};
  if (CAESCipherWrapper::EncryptString(authChallenge, session.GetSessionId(), session.GetAuthString(), expectedResponse))
  {
    if (expectedResponse == challengeResponse)
    {
      // authentication successful
      session.UpdateUserSessionState(UserSessionState::READY);
      response["status"] = "ok";
      return response;
    }
    else
    {
      response["error"] = "authentication_failed";
      return response;
    }
  } else
  {
    response["error"] = "encryption_failed";
    return response;
  }

  return response;
}

} // namespace knocknock