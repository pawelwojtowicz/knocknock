#include <gtest/gtest.h>
#include <CDatabase.h>

std::string dbInterfaceKey = "dbInterface";
std::string dbInterfaceValue = "1.0";


TEST( CSQLiteDriver, SysParam_GetParameter)
{
  DBAccess::CDatabase database;

  database.OpenDatabase("test.db");


  database.GetSystemParamData().AddSystemParam(dbInterfaceKey,dbInterfaceValue);

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
