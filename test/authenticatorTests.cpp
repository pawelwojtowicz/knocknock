#include <gtest/gtest.h>
#include <CSession.h>
#include <CAuthenticator.h>
#include <CConfiguration.h>

using namespace knocknock;

TEST(CAuthenticator, Basic_unknown_failed)
{
  CConfiguration config;

  CSession session("sessionId", "testUser", "", "unknown", "");

  CAuthenticator authenticator;

  authenticator.Initialize(config);

  authenticator.Login(session, "testUser", "");

  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_FAILED);

  authenticator.Shutdown();
}


TEST(CAuthenticator, Basic_sha256_failed)
{
  CConfiguration config;

  CSession session("sessionId", "testUser", "", "sha256", "786FAE789EB7D049E9D00CA71CCEED2127C68E330D286BD7425994E57E5B84D0");

  CAuthenticator authenticator;

  authenticator.Initialize(config);

  authenticator.Login(session, "testUser", "testPassword");

  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_FAILED);

  authenticator.Shutdown();
}

TEST(CAuthenticator, Basic_sha256_success)
{
  CConfiguration config;

  CSession session("sessionId", "testUser", "", "sha256", "786FAE789EB7D049E9D00CA71CCEED2127C68E330D286BD7425994E57E5B84D0");

  CAuthenticator authenticator;

  authenticator.Initialize(config);

  authenticator.Login(session, "testUser", "italianoVero$123");

  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::READY);

  authenticator.Shutdown();
}

TEST(CAuthenticator, Basic_simpledb_success)
{
  CConfiguration config;

  CSession session("sessionId", "testUser", "", "simpledb", "");

  CAuthenticator authenticator;

  authenticator.Initialize(config);

  authenticator.Login(session, "testUser", "");

  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::READY);

  authenticator.Shutdown();
}
