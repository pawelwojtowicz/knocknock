#pragma once
#include "IDBDriver.h"

namespace DBDriver
{
class CSQLiteDriver : public IDBDriver
{
  CSQLiteDriver() = default;
  virtual ~CSQLiteDriver() = default;
  
private:
bool Open( const std::string& dbFilename) override;
void Close() override;


};
}