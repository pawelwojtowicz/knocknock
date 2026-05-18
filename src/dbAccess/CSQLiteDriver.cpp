#include "CSQLiteDriver.h"

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
  
  rc = sqlite3_exec(m_pDBEngine, command.c_str(), callbackFunction, data, &zErrMsg);

  if (zErrMsg)
  {
    sqlite3_free(zErrMsg);
  }

  return SQLITE_OK == rc;
}

bool CSQLiteDriver::ExecutePreparedStatement( const std::string& sql,
                                              const std::vector<std::string>& params,
                                              tSQLiteCallback callbackFunction,
                                              void* data )
{
  sqlite3_stmt* stmt = nullptr;

  if (sqlite3_prepare_v2(m_pDBEngine, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
  {
    return false;
  }

  for (size_t i = 0; i < params.size(); ++i)
  {
    if (sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
    {
      sqlite3_finalize(stmt);
      return false;
    }
  }

  int rc = sqlite3_step(stmt);
  while (rc == SQLITE_ROW)
  {
    if (callbackFunction)
    {
      int colCount = sqlite3_column_count(stmt);
      std::vector<char*> values(colCount);
      std::vector<char*> colNames(colCount);

      for (int col = 0; col < colCount; ++col)
      {
        values[col] = (char*)sqlite3_column_text(stmt, col);
        colNames[col] = (char*)sqlite3_column_name(stmt, col);
      }

      callbackFunction(data, colCount, values.data(), colNames.data());
    }
    rc = sqlite3_step(stmt);
  }

  sqlite3_finalize(stmt);
  return (rc == SQLITE_DONE);
}

}