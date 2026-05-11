#include <gtest/gtest.h>
#include <CLoginRateLimiter.h>
#include <CSessionManager.h>
#include <CDatabase.h>
#include <CSQLiteDriver.h>
#include <CConfiguration.h>
#include <CUser.h>
#include <CTimespan.h>
#include <filesystem>
#include <memory>

using namespace Utilities;
using namespace knocknock;

// ============================================================================
// Unit tests for CLoginRateLimiter in isolation
// ============================================================================

class LoginRateLimiterTests : public ::testing::Test
{
protected:
  CLoginRateLimiter m_rateLimiter;

  virtual void SetUp() override
  {
    m_rateLimiter.Configure(3, 10); // 3 attempts, 10 second base lockout
  }
};

TEST_F(LoginRateLimiterTests, FirstAttemptIsAlwaysAllowed)
{
  EXPECT_TRUE(m_rateLimiter.IsLoginAllowed("user1", 100));
}

TEST_F(LoginRateLimiterTests, AttemptsAllowedBelowThreshold)
{
  m_rateLimiter.RecordFailure("user1", 100);
  EXPECT_TRUE(m_rateLimiter.IsLoginAllowed("user1", 101));

  m_rateLimiter.RecordFailure("user1", 101);
  EXPECT_TRUE(m_rateLimiter.IsLoginAllowed("user1", 102));
}

TEST_F(LoginRateLimiterTests, LockedOutAfterMaxAttempts)
{
  m_rateLimiter.RecordFailure("user1", 100);
  m_rateLimiter.RecordFailure("user1", 101);
  m_rateLimiter.RecordFailure("user1", 102);

  // Should be locked out now (3 failures = max)
  EXPECT_FALSE(m_rateLimiter.IsLoginAllowed("user1", 103));
}

TEST_F(LoginRateLimiterTests, LockoutExpiresAfterDuration)
{
  m_rateLimiter.RecordFailure("user1", 100);
  m_rateLimiter.RecordFailure("user1", 101);
  m_rateLimiter.RecordFailure("user1", 102); // lockout starts: 102 + 10 = 112

  EXPECT_FALSE(m_rateLimiter.IsLoginAllowed("user1", 105));
  EXPECT_FALSE(m_rateLimiter.IsLoginAllowed("user1", 111));

  // Lockout should expire at 112
  EXPECT_TRUE(m_rateLimiter.IsLoginAllowed("user1", 112));
  EXPECT_TRUE(m_rateLimiter.IsLoginAllowed("user1", 120));
}

TEST_F(LoginRateLimiterTests, ExponentialBackoff)
{
  // First lockout: base = 10s
  m_rateLimiter.RecordFailure("user1", 100);
  m_rateLimiter.RecordFailure("user1", 101);
  m_rateLimiter.RecordFailure("user1", 102); // lockout: 102 + 10 = 112

  EXPECT_FALSE(m_rateLimiter.IsLoginAllowed("user1", 103));
  EXPECT_TRUE(m_rateLimiter.IsLoginAllowed("user1", 112));

  // 4th failure (1 excess): lockout = 10 * 2^1 = 20s
  m_rateLimiter.RecordFailure("user1", 113); // lockout: 113 + 20 = 133

  EXPECT_FALSE(m_rateLimiter.IsLoginAllowed("user1", 114));
  EXPECT_FALSE(m_rateLimiter.IsLoginAllowed("user1", 132));
  EXPECT_TRUE(m_rateLimiter.IsLoginAllowed("user1", 133));

  // 5th failure (2 excess): lockout = 10 * 2^2 = 40s
  m_rateLimiter.RecordFailure("user1", 134); // lockout: 134 + 40 = 174

  EXPECT_FALSE(m_rateLimiter.IsLoginAllowed("user1", 135));
  EXPECT_FALSE(m_rateLimiter.IsLoginAllowed("user1", 173));
  EXPECT_TRUE(m_rateLimiter.IsLoginAllowed("user1", 174));
}

TEST_F(LoginRateLimiterTests, SuccessResetsCounter)
{
  m_rateLimiter.RecordFailure("user1", 100);
  m_rateLimiter.RecordFailure("user1", 101);

  // Success resets the counter
  m_rateLimiter.RecordSuccess("user1");

  // Should be allowed again, counter is reset
  EXPECT_TRUE(m_rateLimiter.IsLoginAllowed("user1", 102));

  // Need 3 new failures to trigger lockout again
  m_rateLimiter.RecordFailure("user1", 103);
  m_rateLimiter.RecordFailure("user1", 104);
  EXPECT_TRUE(m_rateLimiter.IsLoginAllowed("user1", 105));

  m_rateLimiter.RecordFailure("user1", 105);
  EXPECT_FALSE(m_rateLimiter.IsLoginAllowed("user1", 106));
}

TEST_F(LoginRateLimiterTests, DifferentUsersAreIndependent)
{
  m_rateLimiter.RecordFailure("user1", 100);
  m_rateLimiter.RecordFailure("user1", 101);
  m_rateLimiter.RecordFailure("user1", 102);

  // user1 is locked out
  EXPECT_FALSE(m_rateLimiter.IsLoginAllowed("user1", 103));

  // user2 is unaffected
  EXPECT_TRUE(m_rateLimiter.IsLoginAllowed("user2", 103));
}

TEST_F(LoginRateLimiterTests, CleanupRemovesExpiredEntries)
{
  m_rateLimiter.RecordFailure("user1", 100);
  m_rateLimiter.RecordFailure("user1", 101);
  m_rateLimiter.RecordFailure("user1", 102); // lockout expires at 112

  EXPECT_FALSE(m_rateLimiter.IsLoginAllowed("user1", 103));

  // Cleanup after lockout expires
  m_rateLimiter.Cleanup(113);

  // After cleanup, user is allowed (entry removed)
  EXPECT_TRUE(m_rateLimiter.IsLoginAllowed("user1", 113));

  // And the counter is fully reset (need 3 new failures)
  m_rateLimiter.RecordFailure("user1", 114);
  m_rateLimiter.RecordFailure("user1", 115);
  EXPECT_TRUE(m_rateLimiter.IsLoginAllowed("user1", 116));
}

TEST_F(LoginRateLimiterTests, BackoffCappedAt64x)
{
  // Configure with 1 attempt, 10s base for easier testing
  CLoginRateLimiter limiter;
  limiter.Configure(1, 10);

  // Trigger many failures to test cap
  for (int i = 0; i < 10; ++i)
  {
    limiter.RecordFailure("user1", 1000 + i * 1000);
  }

  // The cap is 6 (2^6 = 64), so max lockout = 10 * 64 = 640s
  // After 10 failures with maxAttempts=1, excess = 9, but capped at 6
  // Last failure at time 10000, lockout = 10000 + 640 = 10640
  EXPECT_FALSE(limiter.IsLoginAllowed("user1", 10639));
  EXPECT_TRUE(limiter.IsLoginAllowed("user1", 10640));
}

// ============================================================================
// Integration tests: rate limiting through CSessionManager
// ============================================================================

static std::string rateLimitTestDBFileName = "rateLimiter_test.db";

class SessionManagerRateLimitTests : public ::testing::Test
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

    CTimespan::SetTimeline(0);

    if (std::filesystem::exists(rateLimitTestDBFileName))
    {
      std::filesystem::remove(rateLimitTestDBFileName);
    }

    m_database.OpenDatabase(rateLimitTestDBFileName);

    // user with sha256 auth - password "buongiorno$123" hashes to the stored value
    m_database.GetUserData().AddUser(CUser("testuser", "Test", "User", "sha256", "F0295AD01C66AF4BFCFFF941F0E60AC230EA4784DA7A8347EDC31394490384D2"));
    // user with simpledb auth (always succeeds)
    m_database.GetUserData().AddUser(CUser("easyuser", "Easy", "User", "simpledb", ""));

    m_configuration.LoadConfig(m_database);

    m_pSessionManager = std::make_shared<CSessionManager>(m_database, m_configuration);
    ASSERT_TRUE(m_pSessionManager->Initialize());

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

    if (std::filesystem::exists(rateLimitTestDBFileName))
    {
      std::filesystem::remove(rateLimitTestDBFileName);
    }

    CTimespan::SetTimeline(-1);
  }
};

TEST_F(SessionManagerRateLimitTests, LoginBlockedAfterMaxFailedAttempts)
{
  // Default config: maxLoginAttempts=5, loginLockoutSeconds=30
  // Attempt login with wrong password repeatedly
  for (int i = 0; i < 5; ++i)
  {
    tKeyValueMap input = {{"userId", "testuser"}, {"password", "wrongpassword"}};
    tKeyValueMap output;
    const CSession session = m_knocknockService->Login(input, output);
    EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_FAILED);
  }

  // 6th attempt should be rate-limited (returns empty session)
  tKeyValueMap input = {{"userId", "testuser"}, {"password", "buongiorno$123"}};
  tKeyValueMap output;
  const CSession session = m_knocknockService->Login(input, output);
  EXPECT_EQ(session.GetUserSessionState(), UserSessionState::INVALID_SESSION);
  EXPECT_EQ(output["message"], "Too many failed attempts. Try again later.");
}

TEST_F(SessionManagerRateLimitTests, LoginAllowedAfterLockoutExpires)
{
  // Trigger lockout
  for (int i = 0; i < 5; ++i)
  {
    tKeyValueMap input = {{"userId", "testuser"}, {"password", "wrongpassword"}};
    tKeyValueMap output;
    m_knocknockService->Login(input, output);
  }

  // Still locked out
  {
    tKeyValueMap input = {{"userId", "testuser"}, {"password", "buongiorno$123"}};
    tKeyValueMap output;
    const CSession session = m_knocknockService->Login(input, output);
    EXPECT_EQ(session.GetUserSessionState(), UserSessionState::INVALID_SESSION);
  }

  // Advance time past lockout (30 seconds)
  CTimespan::AddTimespan(31);

  // Should be allowed now
  {
    tKeyValueMap input = {{"userId", "testuser"}, {"password", "buongiorno$123"}};
    tKeyValueMap output;
    const CSession session = m_knocknockService->Login(input, output);
    EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_SUCCESS);
  }
}

TEST_F(SessionManagerRateLimitTests, SuccessfulLoginResetsRateLimit)
{
  // Accumulate some failures (but not enough to trigger lockout)
  for (int i = 0; i < 4; ++i)
  {
    tKeyValueMap input = {{"userId", "testuser"}, {"password", "wrongpassword"}};
    tKeyValueMap output;
    m_knocknockService->Login(input, output);
  }

  // Successful login resets the counter
  {
    tKeyValueMap input = {{"userId", "testuser"}, {"password", "buongiorno$123"}};
    tKeyValueMap output;
    const CSession session = m_knocknockService->Login(input, output);
    EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_SUCCESS);
  }

  // Should be able to fail 4 more times without lockout
  for (int i = 0; i < 4; ++i)
  {
    tKeyValueMap input = {{"userId", "testuser"}, {"password", "wrongpassword"}};
    tKeyValueMap output;
    const CSession session = m_knocknockService->Login(input, output);
    EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_FAILED);
  }

  // Still not locked out (only 4 failures since reset)
  {
    tKeyValueMap input = {{"userId", "testuser"}, {"password", "buongiorno$123"}};
    tKeyValueMap output;
    const CSession session = m_knocknockService->Login(input, output);
    EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_SUCCESS);
  }
}

TEST_F(SessionManagerRateLimitTests, RateLimitIsPerUser)
{
  // Lock out testuser
  for (int i = 0; i < 5; ++i)
  {
    tKeyValueMap input = {{"userId", "testuser"}, {"password", "wrongpassword"}};
    tKeyValueMap output;
    m_knocknockService->Login(input, output);
  }

  // testuser is locked out
  {
    tKeyValueMap input = {{"userId", "testuser"}, {"password", "buongiorno$123"}};
    tKeyValueMap output;
    const CSession session = m_knocknockService->Login(input, output);
    EXPECT_EQ(session.GetUserSessionState(), UserSessionState::INVALID_SESSION);
  }

  // easyuser is unaffected
  {
    tKeyValueMap input = {{"userId", "easyuser"}};
    tKeyValueMap output;
    const CSession session = m_knocknockService->Login(input, output);
    EXPECT_EQ(session.GetUserSessionState(), UserSessionState::AUTH_SUCCESS);
  }
}
