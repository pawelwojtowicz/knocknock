#include <gtest/gtest.h>
#include <CSessionBuilder.h>
#include <CDatabase.h>
#include <CConfiguration.h>
#include <filesystem>

using namespace knocknock;



void InitializeDB(DBAccess::CDatabase& database)
{
  std::string testDBFileName = "test.db";
  if (std::filesystem::exists(testDBFileName))
  {
    std::filesystem::remove(testDBFileName);
  }
  
  database.OpenDatabase(testDBFileName);

  database.GetUserData().AddUser( CUser( "cashier1", "Cashier", "#1", "simpledb", "" ));
  database.GetUserData().AddUser( CUser( "1234", "John", "Doe", "", "" ));
  database.GetUserData().AddUser( CUser( "4312", "Paul", "Newman", "otp", "1234567890" ));
}

TEST( SessionBuilderTests, NonExistingUser_AnonymousLogingDisallowed )
{
  CConfiguration configuration;
  DBAccess::CDatabase database;

  InitializeDB(database);

  knocknock::CSessionBuilder sessionBuilder(configuration, database);

  std::optional<CSession> session = sessionBuilder.CreateSession("non_existing_user");

  ASSERT_FALSE(session.has_value());
}

TEST( SessionBuilderTests, ExistingUser_AnonymousLogingDisallowed_AuthMethodNotDefined )
{
  CConfiguration configuration;
  DBAccess::CDatabase database;

  InitializeDB(database);

  knocknock::CSessionBuilder sessionBuilder(configuration, database);

  std::optional<CSession> session = sessionBuilder.CreateSession("1234");

  ASSERT_TRUE(session.has_value());
  ASSERT_EQ(session->GetUserId(), "1234");
  ASSERT_EQ(session->GetUserName(), "John Doe");
  ASSERT_EQ(session->GetAuthMethod(), "sha256");
  ASSERT_EQ(session->GetAuthString(), "db89a15ca72c6c91a94c03e6b7973bbbf01b3e67988c9f79d6b764b36d913a66");
}

TEST( SessionBuilderTests, ExistingUser_AnonymousLogingDisallowed_AuthMethodDefined )
{
  CConfiguration configuration;
  DBAccess::CDatabase database;

  InitializeDB(database);

  knocknock::CSessionBuilder sessionBuilder(configuration, database);

  std::optional<CSession> session = sessionBuilder.CreateSession("4312");

  ASSERT_TRUE(session.has_value());
  ASSERT_EQ(session->GetUserId(), "4312");
  ASSERT_EQ(session->GetUserName(), "Paul Newman");
  ASSERT_EQ(session->GetAuthMethod(), "otp");
  ASSERT_EQ(session->GetAuthString(), "1234567890");
}

TEST( SessionBuilderTests, NonExistingUser1_AnonymousLoginAllowed )
{
  DBAccess::CDatabase database;
  InitializeDB(database);
  database.GetSystemParamData().AddSystemParam("anonymousUserTemplate", "cashier1");

  CConfiguration configuration;
  configuration.LoadConfig(database);

  knocknock::CSessionBuilder sessionBuilder(configuration, database);

  std::optional<CSession> session = sessionBuilder.CreateSession("non_existing_user");

  ASSERT_TRUE(session.has_value());
  ASSERT_EQ(session->GetUserId(), "non_existing_user");
  ASSERT_EQ(session->GetUserName(), "Cashier #1");
  ASSERT_EQ(session->GetAuthMethod(), "simpledb");
  ASSERT_EQ(session->GetAuthString(), configuration.GetDefaultAuthenticationString());
}

TEST( SessionBuilderTests, NonExistingUser2_AnonymousLoginAllowed )
{
  DBAccess::CDatabase database;
  InitializeDB(database);
  database.GetSystemParamData().AddSystemParam("anonymousUserTemplate", "cashier1");

  CConfiguration configuration;
  configuration.LoadConfig(database);

  knocknock::CSessionBuilder sessionBuilder(configuration, database);

  std::optional<CSession> session = sessionBuilder.CreateSession("buffalo_bob");

  ASSERT_TRUE(session.has_value());
  ASSERT_EQ(session->GetUserId(), "buffalo_bob");
  ASSERT_EQ(session->GetUserName(), "Cashier #1");
  ASSERT_EQ(session->GetAuthMethod(), "simpledb");
  ASSERT_EQ(session->GetAuthString(), configuration.GetDefaultAuthenticationString());
}

TEST( SessionBuilderTests, ExistingUser_AnonymousLoginAllowed )
{
  DBAccess::CDatabase database;
  InitializeDB(database);
  database.GetSystemParamData().AddSystemParam("anonymousUserTemplate", "cashier1");

  CConfiguration configuration;
  configuration.LoadConfig(database);

  knocknock::CSessionBuilder sessionBuilder(configuration, database);

  std::optional<CSession> session = sessionBuilder.CreateSession("4312");

  ASSERT_TRUE(session.has_value());
  ASSERT_EQ(session->GetUserId(), "4312");
  ASSERT_EQ(session->GetUserName(), "Paul Newman");
  ASSERT_EQ(session->GetAuthMethod(), "otp");
  ASSERT_EQ(session->GetAuthString(), "1234567890");
}
