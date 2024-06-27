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

bool CSQLiteDriver::ExecuteSQLCommand( const std::string& command, tSQLiteCallback callbackFunction )
{
  int rc = { 0 };
  char *zErrMsg = 0;
  const char* data = "Callback function called";

  auto commandCallback = [](void *data, int argc, char **argv, char **azColName) {
    CSQLiteDriver* driver = (CSQLiteDriver*)data;
    driver->m_data.clear();
    std::vector<std::string> row;
    for( int i = 0 ; i < argc ; ++i)
    {
      row.push_back(std::string(argv[i]));
      std::cout << "temp" << std::endl;
    }
    if ( row.size() > 0 )
    {
      std::cout << "adaduka" << std::endl;

      driver->m_data.push_back( row );
    }
    return 0;
  };

  rc = sqlite3_exec(m_pDBEngine, command.c_str(), commandCallback, (void*)this, &zErrMsg);

  std::cout << "["<<command << "To jest result " << rc << std::endl;
  if ( 0 == rc )
  {
    for ( auto& row : m_data )
    {
#pragma message ("shitty for now")
      callbackFunction(row);
    }
    return true;
  }
  return false;
}

}