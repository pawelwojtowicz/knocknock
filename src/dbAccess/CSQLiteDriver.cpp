#include "CSQLiteDriver.h"
#include <iostream>

namespace DBAccess
{

bool CSQLiteDriver::Open( const std::string& dbFilename)
{
  int rc = { 0 };

  rc = sqlite3_open(dbFilename.c_str(), &m_pDBEngine);

  if( rc ) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(m_pDBEngine));
    return( false );
  }
  return true;
}

void CSQLiteDriver::Close()
{
  sqlite3_close(m_pDBEngine);
}

bool CSQLiteDriver::ExecuteSQLCommand( const std::string& command, tSQLiteCallback callbackFunction, void* data )
{
  int rc = { 0 };
  char *zErrMsg = 0;

  //std::cout << command << std::endl;  

  rc = sqlite3_exec(m_pDBEngine, command.c_str(), callbackFunction, data, &zErrMsg);

  return SQLITE_OK == rc;
}

}