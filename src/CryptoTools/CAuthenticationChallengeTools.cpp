#include "CAuthenticationChallengeTools.h"
#include <time.h>
#include "CSHA256Hash.h"

namespace knocknock
{

std::string CAuthenticationChallengeTools::GenerateAuthenticationChallenge( const std::string& userId)
{
  time_t _tm =time(NULL );
  struct tm * curtime = localtime ( &_tm );

  return ( userId + " " +asctime(curtime) );
}

std::string CAuthenticationChallengeTools::GenerateSessionId( const std::string& userId)
{
  std::string rawChallenge = GenerateAuthenticationChallenge(userId);
  std::string sessionId = {};
  std::string sessionIdRaw = {};

  if (CSHA256Hash::CalculateHash(rawChallenge, sessionId, sessionIdRaw))
  {
    return sessionId;
  }
  return std::string{};
}

}