#pragma once
#include "IDBDriver.h"
#include <sqlite3.h>

namespace DBAccess
{
class CSQLiteDriver : public IDBDriver
{
public:
  CSQLiteDriver() = default;
  virtual ~CSQLiteDriver() = default;

  bool Open( const std::string& dbFilename) override;
  void Close() override;

  bool ExecuteSQLCommand( const std::string& command, tSQLiteCallback callbackFunction, void* data ) override;

  bool ExecutePreparedStatement( const std::string& sql,
                                 const std::vector<std::string>& params,
                                 tSQLiteCallback callbackFunction,
                                 void* data ) override;

private:
  sqlite3 *m_pDBEngine = nullptr;
};
}