#pragma once
#include <string>
#include <map>

namespace knocknock
{
 
using tKeyValueMap = std::map<std::string, std::string>;
using tKeyValueMapIterator = tKeyValueMap::iterator;
using tKeyValueMapConstIterator = tKeyValueMap::const_iterator;

enum class UserSessionState
{
  CREATED,           // created a session for an endpoint (with UserId)
  AUTH_IN_PROGRESS,  // authentication process is in progress (under the control of the auth method implementation )
                     // The "AUTH_IN_PROGRESS" state can be skipped (switching directly from CREATED to AUTH_SUCCESS),
                     // whenever the auth method allows it
  AUTH_SUCCESS,      // the authentication process has been completed successfully
  AUTH_FAILED,       // The authentication process has failed, the session will be removed purged
 // READY,             // The session has been properly authenticated - only the READY sessions are published to the external
                     // world (for example through the express session)
  EXPIRED,           // The session has expired and is no longer valid, the session will be purged
  LOGGED_OUT         // The session has been logged out, the session will be purged
};

enum class CookieSameSite
{
  STRICT,
  LAX,
  NONE
};

enum class HTTPReturnCodes
{
  OK = 200, // the login request successful, either after login or authentication   
  CREATED = 201,// the session has been created (after login)
  NO_CONTENT = 204,
  BAD_REQUEST = 400,
  UNAUTHORIZED = 401,
  FORBIDDEN = 403,
  NOT_FOUND = 404,
  CONFLICT = 409,
  INTERNAL_SERVER_ERROR = 500,
  SERVICE_UNAVAILABLE = 503
};

} // namespace knocknock  