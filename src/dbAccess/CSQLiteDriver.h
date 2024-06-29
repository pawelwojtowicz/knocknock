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

private:
  bool ExecuteSQLCommand( const std::string& command, tSQLiteCallback callbackFunction, void* data ) override;

private:
  sqlite3 *m_pDBEngine = nullptr;
};
}