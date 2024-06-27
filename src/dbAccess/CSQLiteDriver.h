#pragma once
#include "IDBDriver.h"
#include <sqlite3.h>

namespace DBDriver
{
class CSQLiteDriver : public IDBDriver
{
public:
  CSQLiteDriver() = default;
  virtual ~CSQLiteDriver() = default;
  
private:
  bool Open( const std::string& dbFilename) override;
  void Close() override;

  bool ExecuteSQLCommand( const std::string& command, tSQLiteCallback callbackFunction ) override;

private:
  sqlite3 *m_pDBEngine = nullptr;
};
}