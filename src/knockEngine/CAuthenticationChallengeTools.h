#pragma once
#include <string>

namespace knocknock
{

class CAuthenticationChallengeTools
{
  CAuthenticationChallengeTools() = delete;
  ~CAuthenticationChallengeTools() = delete;
public:
  static std::string GenerateAuthenticationChallenge( const std::string& userId);
  static std::string GenerateSessionId( const std::string& userId);

};
}