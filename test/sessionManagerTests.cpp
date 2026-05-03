#include <gtest/gtest.h>
#include <CSessionManager.h>
#include <CDatabase.h>
#include <CSQLiteDriver.h>
#include <CConfiguration.h>
#include <CUser.h>
#include <CTimespan.h>
#include <filesystem>
#include <CSHA256Hash.h>
#include <CAESCipherWrapper.h>
#include <memory>

using namespace Utilities;

using namespace knocknock;

static std::string testDBFileName = "sessionManager_test.db";

class SessionManagerTests : public ::testing::Test
{
protected:
  DBAccess::CSQLiteDriver m_dbDriver;
  DBAccess::CDatabase m_database{m_dbDriver};
  CConfiguration m_configuration;
  std::shared_ptr<CSessionManager> m_pSessionManager;
  std::shared_ptr<IKnocknockService> m_knocknockService;

  virtual void SetUp() override
  {
    m_configuration.LoadConfig("config/knocknock.conf");

    CTimespan::SetTimeline(0); // reset time to real current time
    // Clean up any existing test database
    if (std::filesystem::exists(testDBFileName))
    {
      std::filesystem::remove(testDBFileName);
    }

    // Initialize database
    m_database.OpenDatabase(testDBFileName);

    // Add test users
    m_database.GetUserData().AddUser(CUser("user1", "First", "User", "sha256", "F0295AD01C66AF4BFCFFF941F0E60AC230EA4784DA7A8347EDC31394490384D2"));
    m_database.GetUserData().AddUser(CUser("user2", "Second", "User", "otp", "123456"));
    m_database.GetUserData().AddUser(CUser("user3", "Third", "User", "simpledb", ""));
    m_database.GetUserData().AddUser(CUser("chuck", "Chuck", "Norris", "scr", "F0295AD01C66AF4BFCFFF941F0E60AC230EA4784DA7A8347EDC31394490384D2"));

    // Add roles
    // TODO: Add roles as needed

    // Add privileges
    // TODO: Add privileges as needed

    // Load configuration
    m_configuration.LoadConfig(m_database);

    // Create session manager
    m_pSessionManager = std::make_shared<CSessionManager>(m_database, m_configuration);
    ASSERT_TRUE(m_pSessionManager->Initialize());
    
    //for publishing in test cases
    m_knocknockService = m_pSessionManager;

  }

  virtual void TearDown() override
  {
    if (m_pSessionManager)
    {
      m_pSessionManager->Shutdown();
      m_pSessionManager.reset();
    }

    m_database.Close();

    // Clean up test database
    if (std::filesystem::exists(testDBFileName))
    {
      //std::filesystem::remove(testDBFileName);
    }

    CTimespan::SetTimeline(-1); // reset time to real current time
  }
};

// Test cases using the fixture
TEST_F(SessionManagerTests, Initialization)
{
  EXPECT_TRUE(m_pSessionManager != nullptr);
}

TEST_F(SessionManagerTests, Login_ValidUser)
{
  tKeyValueMap input = {{"userId", "user3"}};
  tKeyValueMap output;
  const CSession session = m_knocknockService->Login(input, output);

  EXPECT_EQ(session.GetUserId(), "user3");
  EXPECT_EQ(session.GetUserName(), "Third User");
  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_SUCCESS);


}

TEST_F(SessionManagerTests, Login_InvalidUser)
{
  tKeyValueMap input = {{"userId", "user4"}};
  tKeyValueMap output;
  const CSession session = m_knocknockService->Login(input, output);

  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::INVALID_SESSION);
  EXPECT_TRUE(session.GetSessionId().empty());
  EXPECT_TRUE(session.GetUserId().empty());
}

TEST_F(SessionManagerTests, Login_ValidUser_Logoff)
{
  tKeyValueMap input = {{"userId", "user3"}};
  tKeyValueMap output;
  const CSession session = m_knocknockService->Login(input, output);

  EXPECT_EQ(session.GetUserId(), "user3");
  EXPECT_EQ(session.GetUserName(), "Third User");
  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_SUCCESS);

  // Now log off
  tKeyValueMap logoutInput = {{"sessionId", session.GetSessionId()}};
  tKeyValueMap logoutOutput;
  bool logoutResult = m_knocknockService->Logout(logoutInput, logoutOutput);
  
  EXPECT_TRUE(logoutResult);
}

TEST_F(SessionManagerTests, Login_ValidUser_Timeout200_Logoff)
{
  tKeyValueMap input = {{"userId", "user3"}};
  tKeyValueMap output;
  const CSession session = m_knocknockService->Login(input, output);

  EXPECT_EQ(session.GetUserId(), "user3");
  EXPECT_EQ(session.GetUserName(), "Third User");
  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_SUCCESS);

  // Simulate time passing to trigger session timeout
  CTimespan::AddTimespan(200); // Add 200 seconds to the timeline

  // Now log off
  tKeyValueMap logoutInput = {{"sessionId", session.GetSessionId()}};
  tKeyValueMap logoutOutput;
  bool logoutResult = m_knocknockService->Logout(logoutInput, logoutOutput);
  
  EXPECT_FALSE(logoutResult);
}

TEST_F(SessionManagerTests, Login_ValidUser_Touching)
{
  tKeyValueMap input = {{"userId", "user3"}};
  tKeyValueMap output;
  const CSession session = m_knocknockService->Login(input, output);

  EXPECT_EQ(session.GetUserId(), "user3");
  EXPECT_EQ(session.GetUserName(), "Third User");
  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_SUCCESS);
  EXPECT_EQ(session.GetSessionExpires(),  180); // default session expiration timeout is 300 seconds

  // Simulate time passing to trigger session timeout
  CTimespan::AddTimespan(100); // Add 200 seconds to the timeline

  // Now log off
  tKeyValueMap logoutInput = {{"sessionId", session.GetSessionId()}};
  tKeyValueMap logoutOutput;
  const CSession touchedSession =  m_knocknockService->Touch(logoutInput, logoutOutput);

  EXPECT_EQ(touchedSession.GetUserSessionState(), UserSessionState::AUTH_SUCCESS);
  EXPECT_EQ(touchedSession.GetSessionExpires(), 280); // session expiration should be extended 
  
  CTimespan::AddTimespan(200); // Add 200 seconds to the timeline
  const CSession touchedSession2 =  m_knocknockService->Touch(logoutInput, logoutOutput);

  EXPECT_EQ(touchedSession2.GetUserSessionState(), UserSessionState::INVALID_SESSION); // session should be expired
}

TEST_F(SessionManagerTests, Login_sha256AuthMethod_ValidUser_Logoff)
{
  tKeyValueMap input = {{"userId", "user1"}, {"password", "buongiorno$123"}};

  tKeyValueMap output;
  const CSession session = m_knocknockService->Login(input, output);

  EXPECT_EQ(session.GetUserId(), "user1");
  EXPECT_EQ(session.GetUserName(), "First User");
  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_SUCCESS);

  // Now log off
  tKeyValueMap logoutInput = {{"sessionId", session.GetSessionId()}};
  tKeyValueMap logoutOutput;
  bool logoutResult = m_knocknockService->Logout(logoutInput, logoutOutput);
  
  EXPECT_TRUE(logoutResult);
}

TEST_F(SessionManagerTests, Login_scrAuthMethod_ValidUser_Logoff)
{
  tKeyValueMap input = {{"userId", "chuck"}};

  tKeyValueMap output;
  const CSession session = m_knocknockService->Login(input, output);

  EXPECT_EQ(session.GetUserId(), "chuck");
  EXPECT_EQ(session.GetUserName(), "Chuck Norris");
  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_IN_PROGRESS);

  std::string sessionId = session.GetSessionId();
  std::string challenge = output["challenge"];

  std::string passwordHash = {};
  std::string passwordHashHex = {};

  CSHA256Hash::CalculateHash("buongiorno$123", passwordHash, passwordHashHex);

  std::string challengeResponse = {};
  CAESCipherWrapper::EncryptString(challenge, sessionId, passwordHashHex, challengeResponse);

  tKeyValueMap authInput = {{"sessionId", sessionId}, {"challenge_response", challengeResponse}};
  tKeyValueMap authOutput;
  const CSession authSession = m_knocknockService->Authenticate(authInput, authOutput); 

  EXPECT_EQ(authSession.GetUserSessionState(), UserSessionState::AUTH_SUCCESS);
  // Now log off
  tKeyValueMap logoutInput = {{"sessionId", session.GetSessionId()}};
  tKeyValueMap logoutOutput;
  bool logoutResult = m_knocknockService->Logout(logoutInput, logoutOutput);  
}

TEST_F(SessionManagerTests, Login_scrAuthMethod_TimeoutDuringAuthentication_ValidUser_Logoff)
{
  tKeyValueMap input = {{"userId", "chuck"}};

  tKeyValueMap output;
  const CSession session = m_knocknockService->Login(input, output);

  EXPECT_EQ(session.GetUserId(), "chuck");
  EXPECT_EQ(session.GetUserName(), "Chuck Norris");
  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_IN_PROGRESS);

  std::string sessionId = session.GetSessionId();
  std::string challenge = output["challenge"];

  std::string passwordHash = {};
  std::string passwordHashHex = {};

  CSHA256Hash::CalculateHash("buongiorno$123", passwordHash, passwordHashHex);

  std::string challengeResponse = {};
  CAESCipherWrapper::EncryptString(challenge, sessionId, passwordHashHex, challengeResponse);

  CTimespan::AddTimespan(10);
  
  tKeyValueMap authInput = {{"sessionId", sessionId}, {"challenge_response", challengeResponse}};
  tKeyValueMap authOutput;
  const CSession authSession = m_knocknockService->Authenticate(authInput, authOutput); 

  EXPECT_EQ(authSession.GetUserSessionState(), UserSessionState::INVALID_SESSION);
}