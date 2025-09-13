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
  AUTHENTICATING,    // authentication process is in progress (under the control of the auth method implementation )
                     // The "AUTHENTICATING" state can be skipped (switching directly from CREATED to READY),
                     // whenever the auth method allows it
  AUTH_FAILED,       // The authentication process has failed, the session will be removed purged
  READY,             // The session has been properly authenticated - only the READY sessions are published to the external
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

}