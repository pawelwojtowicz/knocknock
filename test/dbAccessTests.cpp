#include <gtest/gtest.h>
#include <CSQLiteDriver.h>

TEST( CSQLiteDriver, Basic)
{
  DBDriver::CSQLiteDriver sqliteDBDriver;
  DBDriver::IDBDriver& driver(sqliteDBDriver);

  driver.Open( "test.db");
  driver.Close();
}