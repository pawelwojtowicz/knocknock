#include "CSQLiteDriver.h"

namespace DBDriver
{

bool CSQLiteDriver::Open( const std::string& dbFilename)
{
  int rc = { 0 };

  rc = sqlite3_open("test.db", &m_pDBEngine);

  if( rc ) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(m_pDBEngine));
    return( false );
  }
  return false;
}

void CSQLiteDriver::Close()
{
  sqlite3_close(m_pDBEngine);
}

bool CSQLiteDriver::ExecuteSQLCommand( const std::string& command, tSQLiteCallback callbackFunction )
{
  int rc = { 0 };
  char *zErrMsg = 0;
  const char* data = "Callback function called";

  rc = sqlite3_exec(m_pDBEngine, command.c_str(), callbackFunction, (void*)data, &zErrMsg);

  if ( 0 == rc )
  {
    return true;
  }
  return false;
}

}