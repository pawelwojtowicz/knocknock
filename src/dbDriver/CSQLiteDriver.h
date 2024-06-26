#pragma once
#include "IDBDriver.h"
#include <sqlite3.h>

namespace DBDriver
{
class CSQLiteDriver : public IDBDriver
{
  CSQLiteDriver() = default;
  virtual ~CSQLiteDriver() = default;
  
private:
  bool Open( const std::string& dbFilename) override;
  void Close() override;


private:
  sqlite3 *m_pDBEngine;
};
}