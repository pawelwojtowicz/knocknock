#include <gtest/gtest.h>
#include <CSession.h>
#include <CAuthenticator.h>
#include <CConfiguration.h>
#include <CAESCipherWrapper.h>
#include <iostream>

using namespace knocknock;

constexpr const char* TEST_USER_ID = "TestUser";
// password: italianoVero$123
constexpr const char* TEST_SHA256_PASSWORD1 = "786FAE789EB7D049E9D00CA71CCEED2127C68E330D286BD7425994E57E5B84D0";
// password: testPassword
constexpr const char* TEST_SHA256_PASSWORD2 = "FD5CB51BAFD60F6FDBEDDE6E62C473DA6F247DB271633E15919BAB78A02EE9EB";

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

  CSession session("sessionId", "testUser", "", "sha256", TEST_SHA256_PASSWORD1);

  CAuthenticator authenticator;

  authenticator.Initialize(config);

  // the correct password is italianoVero$123
  authenticator.Login(session, "testPassword");

  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_FAILED);

  authenticator.Shutdown();
}

TEST(CAuthenticator, Basic_sha256_success)
{
  CConfiguration config;

  CSession session("sessionId", "testUser", "", "sha256", TEST_SHA256_PASSWORD1);

  CAuthenticator authenticator;
  authenticator.Initialize(config);

  authenticator.Login(session, "italianoVero$123");

  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_SUCCESS);

  authenticator.Shutdown();
}

TEST(CAuthenticator, Basic_simpledb_success)
{
  CConfiguration config;

  CSession session("sessionId", "testUser", "", "simpledb", "");

  CAuthenticator authenticator;

  authenticator.Initialize(config);

  authenticator.Login(session, "");

  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_SUCCESS);

  authenticator.Shutdown();
}

TEST(CAuthenticator, Basic_scr_success)
{
  CConfiguration config;
  CAuthenticator authenticator;
  authenticator.Initialize(config);

  std::string sessionId{"6E7B30E57AC2B2E6A9FF079F623FE9D892CDD760227070FF2352CABEE60CEEFD"};
  std::string passwordHash{TEST_SHA256_PASSWORD1};
  //create a user session with SCR auth method and password key
  CSession session(sessionId, "TestUser","Herr Test User", "scr", passwordHash);

  //Initiate the logic procedure - should create the challenge and set the session state to AUTH_IN_PROGRESS
  const auto& response = authenticator.Login(session, "");
  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_IN_PROGRESS);


  const auto itChallenge = response.find("challenge");
  ASSERT_TRUE(itChallenge != response.end());
  const std::string& challenge = itChallenge->second;
  ASSERT_FALSE(challenge.empty());
  //return to client the challenge - which needs to be encrypted with the sessionId (as IV) and passwordHash (as key)

  //prepare the response to the challenge
  std::string challengeResponse{};
  CAESCipherWrapper::EncryptString(challenge, sessionId, passwordHash, challengeResponse);

  //pack the respose to "challange_response" key and call the Authenticate method
  tKeyValueMap authPayload{ {"challenge_response", challengeResponse} };
  const auto& authResponse = authenticator.Authenticate(session, authPayload);

  //session shall be in AUTH_SUCCESS state
  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_SUCCESS);

  authenticator.Shutdown();
}

TEST(CAuthenticator, Basic_scr_failure)
{
  CConfiguration config;
  CAuthenticator authenticator;
  authenticator.Initialize(config);

  std::string sessionId{"6E7B30E57AC2B2E6A9FF079F623FE9D892CDD760227070FF2352CABEE60CEEFD"};
  std::string passwordHash{TEST_SHA256_PASSWORD1};
  std::string wrongPasswordHash{TEST_SHA256_PASSWORD2};
  CSession session(sessionId, "TestUser","Herr Test User", "scr", passwordHash);

  const auto& response = authenticator.Login(session, "");
  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_IN_PROGRESS);

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

