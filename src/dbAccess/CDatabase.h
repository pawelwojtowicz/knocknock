#pragma once
#include <string>
#include <memory>
#include <CSQLiteDriver.h>

namespace DBAccess
{

class CDatabase
{
public:
  CDatabase();
  virtual ~CDatabase();

  void OpenDatabase( const std::string& dbFilename );
  void Close( );

private:
  std::unique_ptr<IDBDriver> m_dbDriver;
};

}