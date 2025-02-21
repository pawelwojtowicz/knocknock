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

TEST( CSQLiteDriver, RoleToPrivilegeMapping) 
{
  DBAccess::CDatabase database;

  database.OpenDatabase("test.db");

  database.GetRoleData().AddRole( knocknock::CRole("VIEWER", "Can Read"));
  database.GetRoleData().AddRole( knocknock::CRole("WRITER", "Can WRITE"));
  database.GetRoleData().AddRole( knocknock::CRole("ADMIN", "CAN DO EVERYTHING"));

  database.GetPrivilegeData().AddPrivilege(knocknock::CPrivilege("READ","READING DB"));
  database.GetPrivilegeData().AddPrivilege(knocknock::CPrivilege("ADD","ADD RECORD"));
  database.GetPrivilegeData().AddPrivilege(knocknock::CPrivilege("UPDATE","UPDATE RECORD"));
  database.GetPrivilegeData().AddPrivilege(knocknock::CPrivilege("DELETE","DELETE RECORD"));
  database.GetPrivilegeData().AddPrivilege(knocknock::CPrivilege("ADD_TABLE","ADD TABLE"));
  database.GetPrivilegeData().AddPrivilege(knocknock::CPrivilege("PURGE_TABLE","PURGE TABLE"));

  database.GetRole2PrivilegeMappingData().AddPrivilege2Role("VIEWER","READ");
  database.GetRole2PrivilegeMappingData().AddPrivilege2Role("WRITER","ADD");
  database.GetRole2PrivilegeMappingData().AddPrivilege2Role("WRITER","UPDATE");
  database.GetRole2PrivilegeMappingData().AddPrivilege2Role("WRITER","DELETE");
  database.GetRole2PrivilegeMappingData().AddPrivilege2Role("ADMIN","READ");
  database.GetRole2PrivilegeMappingData().AddPrivilege2Role("ADMIN","ADD");
  database.GetRole2PrivilegeMappingData().AddPrivilege2Role("ADMIN","UPDATE");
  database.GetRole2PrivilegeMappingData().AddPrivilege2Role("ADMIN","DELETE");
  database.GetRole2PrivilegeMappingData().AddPrivilege2Role("ADMIN","ADD_TABLE");
  database.GetRole2PrivilegeMappingData().AddPrivilege2Role("ADMIN","PURGE_TABLE");

  const auto adminPrivileges = database.GetRole2PrivilegeMappingData().GetPrivilegesForRole("ADMIN");
  EXPECT_EQ(adminPrivileges.size(), 6);

  const auto writerPrivileges = database.GetRole2PrivilegeMappingData().GetPrivilegesForRole("WRITER");
  EXPECT_EQ(writerPrivileges.size(), 3);

  const auto viewerPrivileges = database.GetRole2PrivilegeMappingData().GetPrivilegesForRole("VIEWER");
  EXPECT_EQ(viewerPrivileges.size(), 1);
  
  database.Close();
}

TEST( CSQLiteDriver, User2RoleMappings ) 
{
  DBAccess::CDatabase database;

  database.OpenDatabase("test.db");

  database.GetUserData().AddUser(knocknock::CUser( "jckSprw", 
                                                  "Jack", 
                                                  "Sparrow",
                                                  "bush"));

  database.GetUser2RoleMappingData().AssignRoleToUser("jckSprw", "VIEWER");
  const auto userRoles = database.GetUser2RoleMappingData().GetUserRoles("jckSprw");
  const auto userPrivileges = database.GetUser2RoleMappingData().GetUserPrivileges("jckSprw");
  EXPECT_EQ(userRoles.size(),1);
  EXPECT_EQ(userPrivileges.size(), 1 );

  database.GetUser2RoleMappingData().AssignRoleToUser("jckSprw", "WRITER");
  const auto userRoles2 = database.GetUser2RoleMappingData().GetUserRoles("jckSprw");
  const auto userPrivileges2 = database.GetUser2RoleMappingData().GetUserPrivileges("jckSprw");
  EXPECT_EQ(userRoles2.size(),2);
  EXPECT_EQ(userPrivileges2.size(), 4 );

  database.GetUser2RoleMappingData().RemoveRoleFromUser("jckSprw", "VIEWER");
  const auto userRoles3 = database.GetUser2RoleMappingData().GetUserRoles("jckSprw");
  const auto userPrivileges3 = database.GetUser2RoleMappingData().GetUserPrivileges("jckSprw");
  EXPECT_EQ(userRoles3.size(),1);
  EXPECT_EQ(userPrivileges3.size(), 3 );

  
  database.Close();
}

TEST( CSQLiteDriver, Applications ) 
{
  DBAccess::CDatabase database;

  database.OpenDatabase("test.db");

  ASSERT_TRUE(database.GetApplicationData().AddApplication(knocknock::CApplication(-1, "MsWord", "MQTT", "@#$#$#$%@#$%@#$#$%")));
  const auto apps = database.GetApplicationData().GetAllApplications();
  EXPECT_EQ(apps.size(), 1 );

  database.GetApplicationData().AddApplication(knocknock::CApplication(-1, "MsExcel", "MQTT", "@#$#$#$%@#$%@#$#$%"));
  const auto apps1 = database.GetApplicationData().GetAllApplications();
  EXPECT_EQ(apps1.size(), 2 );

  database.GetApplicationData().AddApplication(knocknock::CApplication(-1, "MsAccess", "MQTT", "@#$#$#$%@#$%@#$#$%"));
  const auto apps2 = database.GetApplicationData().GetAllApplications();
  EXPECT_EQ(apps2.size(), 3 );

  const auto& firstRecord = *(apps2.begin());

  
  int recordId = firstRecord.GetAppId();
  const std::string& appNameBeforeChange = firstRecord.GetName();
  //update it and check it
  knocknock::CApplication newAppRecord( recordId, "NewName", "NewPublisher", "NewToken");
  database.GetApplicationData().UpdateApplication(newAppRecord);
  const auto updatedRecordFromDB = database.GetApplicationData().GetApplication(recordId);
  ASSERT_TRUE(updatedRecordFromDB);
  EXPECT_EQ(updatedRecordFromDB->GetName(), "NewName");
  EXPECT_EQ(updatedRecordFromDB->GetDataPublisher(), "NewPublisher");
  EXPECT_EQ(updatedRecordFromDB->GetAccessToken(), "NewToken");

  //remove it and check
  database.GetApplicationData().DeleteApplication(recordId);
  const auto removedRecord = database.GetApplicationData().GetApplication(recordId);
  ASSERT_FALSE(removedRecord);
  
  database.Close();
}