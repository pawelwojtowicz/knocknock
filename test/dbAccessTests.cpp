#include <gtest/gtest.h>
#include <CDatabase.h>
#include <filesystem>
#include "CUser.h"

std::string dbInterfaceKey = "dbInterface";
std::string dbInterfaceValue = "1.0";

std::string testDBFileName = "test.db";

TEST( DBFileCleanup, DeletingTestFile)
{
  if (std::filesystem::exists(testDBFileName))
  {
    std::filesystem::remove( testDBFileName );
  }
  ASSERT_FALSE(std::filesystem::exists(testDBFileName));
}


TEST( CSQLiteDriver, SysParam_AddDuplicates)
{
  DBAccess::CDatabase database;

  database.OpenDatabase("test.db");

  EXPECT_TRUE(database.GetSystemParamData().AddSystemParam(dbInterfaceKey,dbInterfaceValue));
  EXPECT_FALSE(database.GetSystemParamData().AddSystemParam(dbInterfaceKey,dbInterfaceValue));
  database.Close();
}


TEST( CSQLiteDriver, SysParam_GetParameter)
{
  DBAccess::CDatabase database;

  database.OpenDatabase("test.db");

  {
    auto systemParamOpt = database.GetSystemParamData().GetSystemParam(dbInterfaceKey); 

    ASSERT_TRUE( systemParamOpt );
    EXPECT_EQ( dbInterfaceValue, systemParamOpt.value() );
  }

  {
    auto systemParamOpt = database.GetSystemParamData().GetSystemParam("not_existing"); 

    ASSERT_FALSE( systemParamOpt );
  }

  database.Close();
}

TEST( CSQLiteDriver, SysParam_GetAllParams)
{
  DBAccess::CDatabase database;

  database.OpenDatabase("test.db");

  std::string key = "device";
  std::string value = "bayraktar";

  database.GetSystemParamData().AddSystemParam(key,value);

  auto allParamsMap = database.GetSystemParamData().GetAllSystemParams();

  EXPECT_EQ( 2, allParamsMap.size() );
  EXPECT_EQ( value, allParamsMap[key]);
  EXPECT_EQ( dbInterfaceValue, allParamsMap[dbInterfaceKey]);

  database.Close();
}

TEST( CSQLiteDriver, Users_AddingDuplicate )
{
  DBAccess::CDatabase database;

  database.OpenDatabase("test.db");

  knocknock::CUser user("don","John","Doe","1234567890");

  ASSERT_TRUE(database.GetUserData().AddUser(user) );
  ASSERT_FALSE(database.GetUserData().AddUser(user) );

  database.Close();
}

TEST( CSQLiteDriver, Users_GetUserById_Existing )
{
  DBAccess::CDatabase database;

  database.OpenDatabase("test.db");

  auto user = database.GetUserData().GetUserByUserId( "don");

  ASSERT_TRUE( user );
  EXPECT_EQ( "John", user->getFirstName());
  EXPECT_EQ( "Doe", user->getLastName());
  EXPECT_EQ( "1234567890", user->getPasswordHash() );

  database.Close();
}

TEST( CSQLiteDriver, Users_GetUserById_NonExisting )
{
  DBAccess::CDatabase database;

  database.OpenDatabase("test.db");
  auto user = database.GetUserData().GetUserByUserId( "padre");
  ASSERT_FALSE( user );
  database.Close();
}

TEST( CSQLiteDriver, Users_UpdateUser_Existing )
{
  DBAccess::CDatabase database;

  database.OpenDatabase("test.db");
  std::string userId("testUser");
  std::string firstNameBeforeUpdate("Pablo");
  std::string lastNameBeforeUpdate("Morales");
  std::string passHashBeforeChange("1234567890");
  std::string firstNameAfterUpdate("Pietro");
  std::string lastNameAfterUpdate("Desantis");
  std::string passHashAfterChange("0987654321");
  {
    knocknock::CUser user( userId, firstNameBeforeUpdate,lastNameBeforeUpdate,passHashBeforeChange);
    database.GetUserData().AddUser( user );
  }
  {
    knocknock::CUser user( userId, firstNameAfterUpdate,lastNameAfterUpdate,passHashAfterChange);
    ASSERT_TRUE(database.GetUserData().UpdateUser( user) );
  }

  auto user = database.GetUserData().GetUserByUserId( userId );
  ASSERT_TRUE( user );
  EXPECT_EQ( firstNameAfterUpdate, user->getFirstName() );
  EXPECT_EQ( lastNameAfterUpdate, user->getLastName() );
  EXPECT_EQ( passHashAfterChange, user->getPasswordHash() );
  database.Close();
}

TEST( CSQLiteDriver, Users_GetAllUsers )
{
  DBAccess::CDatabase database;

  database.OpenDatabase("test.db");
  const auto users = database.GetUserData().GetAllUsers();

  EXPECT_EQ( 2, users.size() );
  database.Close();
}

TEST( CSQLiteDriver, Users_DeleteUser )
{
  DBAccess::CDatabase database;

  database.OpenDatabase("test.db");
  database.GetUserData().DeleteUser("testUser");
  auto users = database.GetUserData().GetAllUsers();
  EXPECT_EQ( 1, users.size() );
  database.GetUserData().DeleteUser("don");
  users = database.GetUserData().GetAllUsers();
  EXPECT_EQ( 0, users.size() );  
  database.Close();
}

TEST( CSQLiteDriver, Roles_AddRole )
{
  DBAccess::CDatabase database;

  database.OpenDatabase("test.db");

  ASSERT_TRUE( database.GetRoleData().AddRole(knocknock::CRole("pilot", "Switching on TVs")));
  ASSERT_TRUE( database.GetRoleData().AddRole(knocknock::CRole("fork", "carring the broccoli to your boccia")));

  knocknock::tRoles allRoles = database.GetRoleData().GetAllRoles();
  ASSERT_EQ(allRoles.size(), 2 );

  const auto roleQueryResult =  database.GetRoleData().GetRole("pilot");
  ASSERT_TRUE( roleQueryResult );
  ASSERT_EQ( roleQueryResult->GetName(), "pilot");
  ASSERT_EQ( roleQueryResult->GetDescription(), "Switching on TVs");

  database.GetRoleData().DeleteRole("pilot");
  allRoles = database.GetRoleData().GetAllRoles();
  ASSERT_EQ(allRoles.size(), 1 );

  const auto roleQueryResult2 = database.GetRoleData().GetRole("pilot");
  ASSERT_FALSE( roleQueryResult2 );

  database.GetRoleData().DeleteRole("fork");
  allRoles = database.GetRoleData().GetAllRoles();
  ASSERT_EQ(allRoles.size(), 0 );

  database.Close();
}

TEST( CSQLiteDriver, Roles_UpdateRole )
{
  DBAccess::CDatabase database;

  database.OpenDatabase("test.db");

  ASSERT_TRUE( database.GetRoleData().AddRole(knocknock::CRole("test1", "test description1")));
  ASSERT_TRUE( database.GetRoleData().AddRole(knocknock::CRole("test2", "describing the test #2")));

  knocknock::tRoles allRoles = database.GetRoleData().GetAllRoles();
  ASSERT_EQ(allRoles.size(), 2 );

  const auto roleQueryResult =  database.GetRoleData().GetRole("test1");
  ASSERT_TRUE( roleQueryResult );
  ASSERT_EQ( roleQueryResult->GetName(), "test1");
  ASSERT_EQ( roleQueryResult->GetDescription(), "test description1");

  knocknock::CRole updateRole("test1", "AfterUpdate1");
  database.GetRoleData().UpdateRole(updateRole);

  const auto roleQueryResult2 =  database.GetRoleData().GetRole("test1");
  ASSERT_TRUE( roleQueryResult2 );
  ASSERT_EQ( roleQueryResult2->GetName(), "test1");
  ASSERT_EQ( roleQueryResult2->GetDescription(), "AfterUpdate1");

  database.Close();
}

TEST( CSQLiteDriver, Privileges_CRUD_Privilege) 
{
  DBAccess::CDatabase database;

  database.OpenDatabase("test.db");

  ASSERT_TRUE( database.GetPrivilegeData().AddPrivilege( knocknock::CPrivilege("Sleeping", "Sleeping at night and resting")));
  ASSERT_TRUE( database.GetPrivilegeData().AddPrivilege( knocknock::CPrivilege("Running", "Running is healthy")));
  ASSERT_TRUE( database.GetPrivilegeData().AddPrivilege( knocknock::CPrivilege("Eating", "Eating healthy")));

  knocknock::tPrivilegeArray privileges = database.GetPrivilegeData().GetAllPrivileges();
  ASSERT_EQ(privileges.size(), 3 );

  knocknock::CPrivilege update("Running", "Moving per pedes");
  ASSERT_TRUE(database.GetPrivilegeData().UpdatePrivilege(update));

  const auto privilege = database.GetPrivilegeData().GetPrivilege("Running");
  ASSERT_TRUE( privilege );
  EXPECT_EQ( privilege->GetShortDesc(), "Running");
  EXPECT_EQ( privilege->GetLongDesc(), "Moving per pedes");

  database.GetPrivilegeData().DeletePrivilege("Running");
  knocknock::tPrivilegeArray privileges2 = database.GetPrivilegeData().GetAllPrivileges();
  EXPECT_EQ(privileges2.size(), 2);

  database.GetPrivilegeData().DeletePrivilege("Eating");
  knocknock::tPrivilegeArray privileges3 = database.GetPrivilegeData().GetAllPrivileges();
  EXPECT_EQ(privileges3.size(), 1);

  database.Close();
}