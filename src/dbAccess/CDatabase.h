#pragma once
#include <string>
#include <memory>
#include "CSQLiteDriver.h"
#include "CSystemParamData.h"

namespace DBAccess
{

class CDatabase
{
public:
  CDatabase();
  virtual ~CDatabase();

  void OpenDatabase( const std::string& dbFilename );
  void Close( );

  CSystemParamData& GetSystemParamData() {
    return m_sysParamData;
  }

private:
  CSQLiteDriver m_dbDriver;

  CSystemParamData m_sysParamData;
};

}