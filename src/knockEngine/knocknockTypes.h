#pragma once
#include <string>
#include <map>
#include <vector>

namespace knocknock
{

using tStringVector = std::vector<std::string>;
 
enum class UserSessionState
{
  INVALID_SESSION,   // the session is invalid, for example due to missing or invalid session ID
  CREATED,           // created a session for an endpoint (with UserId)
  AUTH_IN_PROGRESS,  // authentication process is in progress (under the control of the auth method implementation )
                     // The "AUTH_IN_PROGRESS" state can be skipped (switching directly from CREATED to AUTH_SUCCESS),
                     // whenever the auth method allows it
  AUTH_SUCCESS,      // the authentication process has been completed successfully
  AUTH_FAILED,       // The authentication process has failed, the session will be removed purged
  VALID,             // The session has been properly authenticated and crossed checked for compliance with active policies
  EXPIRED,           // The session has expired and is no longer valid, the session will be purged
  LOGGED_OUT         // The session has been logged out, the session will be purged
};

} // namespace knocknock  