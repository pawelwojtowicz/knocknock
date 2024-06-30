#include <gtest/gtest.h>
#include <CDatabase.h>
#include "CUser.h"

std::string dbInterfaceKey = "dbInterface";
std::string dbInterfaceValue = "1.0";


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
