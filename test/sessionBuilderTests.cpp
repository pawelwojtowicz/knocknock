#include <gtest/gtest.h>
#include <CSessionBuilder.h>
#include <CDatabase.h>
#include <CSQLiteDriver.h>
#include <CConfiguration.h>
#include <filesystem>
#include <KnocKnockDictionary.h>

using namespace knocknock;


class SessionBuilderTests : public ::testing::Test
{
protected:
  DBAccess::CSQLiteDriver m_dbDriver;
  DBAccess::CDatabase m_database{m_dbDriver};
  CConfiguration m_configuration;

  virtual void SetUp() override
  {
    std::string testDBFileName = "test.db";
    if (std::filesystem::exists(testDBFileName))
    {
      std::filesystem::remove(testDBFileName);
    }

    m_configuration.LoadConfig("config/knocknock.conf");
    
    m_database.OpenDatabase(testDBFileName);

    m_database.GetUserData().AddUser( CUser( "cashier1", "Cashier", "#1", "simpledb", "" ));
    m_database.GetUserData().AddUser( CUser( "1234", "John", "Doe", "", "" ));
    m_database.GetUserData().AddUser( CUser( "4312", "Paul", "Newman", "otp", "1234567890" ));


  }

  virtual void TearDown() override
  {
    m_database.Close();

    std::string testDBFileName = "test.db";
    if (std::filesystem::exists(testDBFileName))
    {
      std::filesystem::remove(testDBFileName);
    }
  }
};

TEST_F( SessionBuilderTests, NonExistingUser_AnonymousLogingDisallowed )
{
  knocknock::CSessionBuilder sessionBuilder(m_configuration, m_database);
  sessionBuilder.Initialize();

  std::optional<CSession> session = sessionBuilder.CreateSession("non_existing_user");

  ASSERT_FALSE(session.has_value());
}

TEST_F( SessionBuilderTests, ExistingUser_AnonymousLogingDisallowed_AuthMethodNotDefined )
{
  knocknock::CSessionBuilder sessionBuilder(m_configuration, m_database);
  sessionBuilder.Initialize();

  std::optional<CSession> session = sessionBuilder.CreateSession("1234");

  ASSERT_TRUE(session.has_value());
  ASSERT_EQ(session->GetUserId(), "1234");
  ASSERT_EQ(session->GetUserName(), "John Doe");
  ASSERT_EQ(session->GetAuthMethod(), "sha256");
  ASSERT_EQ(session->GetAuthString(), "db89a15ca72c6c91a94c03e6b7973bbbf01b3e67988c9f79d6b764b36d913a66");
}

TEST_F( SessionBuilderTests, ExistingUser_AnonymousLogingDisallowed_AuthMethodDefined )
{
  knocknock::CSessionBuilder sessionBuilder(m_configuration, m_database);
  sessionBuilder.Initialize();

  std::optional<CSession> session = sessionBuilder.CreateSession("4312");

  ASSERT_TRUE(session.has_value());
  ASSERT_EQ(session->GetUserId(), "4312");
  ASSERT_EQ(session->GetUserName(), "Paul Newman");
  ASSERT_EQ(session->GetAuthMethod(), "otp");
  ASSERT_EQ(session->GetAuthString(), "1234567890");
}

TEST_F( SessionBuilderTests, NonExistingUser1_AnonymousLoginAllowed )
{
  m_database.GetSystemParamData().AddSystemParam("anonymousUserTemplate", "cashier1");

  m_configuration.LoadConfig(m_database);

  knocknock::CSessionBuilder sessionBuilder(m_configuration, m_database);
  sessionBuilder.Initialize();

  std::optional<CSession> session = sessionBuilder.CreateSession("non_existing_user");

  ASSERT_TRUE(session.has_value());
  ASSERT_EQ(session->GetUserId(), "non_existing_user");
  ASSERT_EQ(session->GetUserName(), "Cashier #1");
  ASSERT_EQ(session->GetAuthMethod(), "simpledb");
  ASSERT_EQ(session->GetAuthString(), m_configuration.GetParamString(cParamName_DefaultAuthenticationString));
}

TEST_F( SessionBuilderTests, NonExistingUser2_AnonymousLoginAllowed )
{
  m_database.GetSystemParamData().AddSystemParam("anonymousUserTemplate", "cashier1");

  m_configuration.LoadConfig(m_database);

  knocknock::CSessionBuilder sessionBuilder(m_configuration, m_database);
  sessionBuilder.Initialize();

  std::optional<CSession> session = sessionBuilder.CreateSession("buffalo_bob");

  ASSERT_TRUE(session.has_value());
  ASSERT_EQ(session->GetUserId(), "buffalo_bob");
  ASSERT_EQ(session->GetUserName(), "Cashier #1");
  ASSERT_EQ(session->GetAuthMethod(), "simpledb");
  ASSERT_EQ(session->GetAuthString(), m_configuration.GetParamString(cParamName_DefaultAuthenticationString));
}

TEST_F( SessionBuilderTests, ExistingUser_AnonymousLoginAllowed )
{
  m_database.GetSystemParamData().AddSystemParam("anonymousUserTemplate", "cashier1");

  m_configuration.LoadConfig(m_database);

  knocknock::CSessionBuilder sessionBuilder(m_configuration, m_database);
  sessionBuilder.Initialize();

  std::optional<CSession> session = sessionBuilder.CreateSession("4312");

  ASSERT_TRUE(session.has_value());
  ASSERT_EQ(session->GetUserId(), "4312");
  ASSERT_EQ(session->GetUserName(), "Paul Newman");
  ASSERT_EQ(session->GetAuthMethod(), "otp");
  ASSERT_EQ(session->GetAuthString(), "1234567890");
}
