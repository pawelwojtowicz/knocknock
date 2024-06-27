#include <gtest/gtest.h>
#include <CDatabase.h>

TEST( CSQLiteDriver, Basic)
{
  DBAccess::CDatabase database;

  database.OpenDatabase("text.db");

  std::string key = "dbInterface";
  std::string value = "1.0";

  database.GetSystemParamData().AddSystemParam(key,value);

  EXPECT_EQ( value, database.GetSystemParamData().GetSystemParam(key));

  database.Close();
}