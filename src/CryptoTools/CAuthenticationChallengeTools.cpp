#include "CAuthenticationChallengeTools.h"
#include <time.h>
#include <iostream>

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
  return {};
}

}