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

  std::string sql = "DELETE from COMPANY where ID=2; " \
                    "SELECT * from COMPANY";

   char *zErrMsg = 0;
   const char* data = "Callback function called";

  auto functionForSQLite = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  rc = sqlite3_exec(m_pDBEngine, sql.c_str(), functionForSQLite, (void*)data, &zErrMsg);

  return false;
}

void CSQLiteDriver::Close()
{

}

}