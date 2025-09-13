#include <gtest/gtest.h>
#include <CSession.h>
#include <CAuthenticator.h>
#include <CConfiguration.h>
#include <CAESCipherWrapper.h>
#include <iostream>

using namespace knocknock;

TEST(CAuthenticator, Basic_unknown_failed)
{
  CConfiguration config;

  CSession session("sessionId", "testUser", "", "unknown", "");

  CAuthenticator authenticator;

  authenticator.Initialize(config);

  authenticator.Login(session, "");

  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_FAILED);

  authenticator.Shutdown();
}


TEST(CAuthenticator, Basic_sha256_failed)
{
  CConfiguration config;

  CSession session("sessionId", "testUser", "", "sha256", "786FAE789EB7D049E9D00CA71CCEED2127C68E330D286BD7425994E57E5B84D0");

  CAuthenticator authenticator;

  authenticator.Initialize(config);

  authenticator.Login(session, "testPassword");

  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_FAILED);

  authenticator.Shutdown();
}

TEST(CAuthenticator, Basic_sha256_success)
{
  CConfiguration config;

  CSession session("sessionId", "testUser", "", "sha256", "786FAE789EB7D049E9D00CA71CCEED2127C68E330D286BD7425994E57E5B84D0");

  CAuthenticator authenticator;

  authenticator.Initialize(config);

  authenticator.Login(session, "italianoVero$123");

  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::READY);

  authenticator.Shutdown();
}

TEST(CAuthenticator, Basic_simpledb_success)
{
  CConfiguration config;

  CSession session("sessionId", "testUser", "", "simpledb", "");

  CAuthenticator authenticator;

  authenticator.Initialize(config);

  authenticator.Login(session, "");

  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::READY);

  authenticator.Shutdown();
}

TEST(CAuthenticator, Basic_scr_success)
{
  CConfiguration config;
  CAuthenticator authenticator;
  authenticator.Initialize(config);

  std::string sessionId{"6E7B30E57AC2B2E6A9FF079F623FE9D892CDD760227070FF2352CABEE60CEEFD"};
  std::string passwordHash{"FD5CB51BAFD60F6FDBEDDE6E62C473DA6F247DB271633E15919BAB78A02EE9EB"};
  CSession session(sessionId, "TestUser","Herr Test User", "scr", passwordHash);

  const auto& response = authenticator.Login(session, "");
  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTHENTICATING);

  const auto itChallenge = response.find("challenge");
  ASSERT_TRUE(itChallenge != response.end());
  const std::string& challenge = itChallenge->second;
  ASSERT_FALSE(challenge.empty());

  std::string challengeResponse{};
  CAESCipherWrapper::EncryptString(challenge, sessionId, passwordHash, challengeResponse);

  tKeyValueMap authPayload{ {"challenge_response", challengeResponse} };
  const auto& authResponse = authenticator.Authenticate(session, authPayload);

  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::READY);

  authenticator.Shutdown();
}

TEST(CAuthenticator, Basic_scr_failure)
{
  CConfiguration config;
  CAuthenticator authenticator;
  authenticator.Initialize(config);

  std::string sessionId{"6E7B30E57AC2B2E6A9FF079F623FE9D892CDD760227070FF2352CABEE60CEEFD"};
  std::string passwordHash{"FD5CB51BAFD60F6FDBEDDE6E62C473DA6F247DB271633E15919BAB78A02EE9EB"};
  std::string wrongPasswordHash{"786FAE789EB7D049E9D00CA71CCEED2127C68E330D286BD7425994E57E5B84D0"};
  CSession session(sessionId, "TestUser","Herr Test User", "scr", passwordHash);

  const auto& response = authenticator.Login(session, "");
  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTHENTICATING);

  const auto itChallenge = response.find("challenge");
  ASSERT_TRUE(itChallenge != response.end());
  const std::string& challenge = itChallenge->second;
  ASSERT_FALSE(challenge.empty());

  std::string challengeResponse{};
  CAESCipherWrapper::EncryptString(challenge, sessionId, wrongPasswordHash, challengeResponse);

  tKeyValueMap authPayload{ {"challenge_response", challengeResponse} };
  const auto& authResponse = authenticator.Authenticate(session, authPayload);

  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_FAILED);

  authenticator.Shutdown();
}

