#include "CToStringTools.h"

namespace knocknock
{

const char* CToStringTools::SessionState( UserSessionState state )
{
  switch (state)
  {
    case UserSessionState::INVALID_SESSION:  return "INVALID_SESSION";
    case UserSessionState::CREATED:          return "CREATED";
    case UserSessionState::AUTH_IN_PROGRESS: return "AUTH_IN_PROGRESS";
    case UserSessionState::AUTH_SUCCESS:     return "AUTH_SUCCESS";
    case UserSessionState::AUTH_FAILED:      return "AUTH_FAILED";
    case UserSessionState::VALID:            return "VALID";
    case UserSessionState::EXPIRED:          return "EXPIRED";
    case UserSessionState::LOGGED_OUT:       return "LOGGED_OUT";
  }
  return "UNKNOWN";
}

}