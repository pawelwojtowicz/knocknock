#include <gtest/gtest.h>
#include <CAuthenticationChallengeTools.h>

TEST( CAuthenticationChallengeTools, GenerateAuthenticationChallenge )
{
  using namespace knocknock;
  std::string authChallenge = CAuthenticationChallengeTools::GenerateAuthenticationChallenge("Tomasz");

  
}