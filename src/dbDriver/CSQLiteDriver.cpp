#include "CSQLiteDriver.h"

namespace DBDriver
{

bool CSQLiteDriver::Open( const std::string& dbFilename)
{
  int rc = { 0 };

  rc = sqlite3_open("test.db", &m_pDBEngine);

  if( rc ) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(m_pDBEngine));
  return(0);
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }

return false;
}

void CSQLiteDriver::Close()
{

}

}