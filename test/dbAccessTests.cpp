#include <gtest/gtest.h>
#include <CSQLiteDriver.h>

TEST( CSQLiteDriver, Basic)
{
  DBAccess::CSQLiteDriver sqliteDBDriver;
  DBAccess::IDBDriver& driver(sqliteDBDriver);

  driver.Open( "test.db");
  driver.Close();
}