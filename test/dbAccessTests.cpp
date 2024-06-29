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
