#include <gtest/gtest.h>
#include <CSessionManager.h>
#include <CDatabase.h>
#include <CConfiguration.h>
#include <CUser.h>
#include <filesystem>
#include <memory>

using namespace knocknock;

static std::string testDBFileName = "sessionManager_test.db";

class SessionManagerTests : public ::testing::Test
{
protected:
  DBAccess::CDatabase m_database;
  CConfiguration m_configuration;
  std::shared_ptr<CSessionManager> m_pSessionManager;
  std::shared_ptr<IKnocknockService> m_knocknockService;

  virtual void SetUp() override
  {
    // Clean up any existing test database
    if (std::filesystem::exists(testDBFileName))
    {
      std::filesystem::remove(testDBFileName);
    }

    // Initialize database
    m_database.OpenDatabase(testDBFileName);

    // Add test users
    m_database.GetUserData().AddUser(CUser("user1", "First", "User", "sha256", "db89a15ca72c6c91a94c03e6b7973bbbf01b3e67988c9f79d6b764b36d913a66"));
    m_database.GetUserData().AddUser(CUser("user2", "Second", "User", "otp", "123456"));
    m_database.GetUserData().AddUser(CUser("user3", "Third", "User", "simpledb", ""));

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
      std::filesystem::remove(testDBFileName);
    }
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
