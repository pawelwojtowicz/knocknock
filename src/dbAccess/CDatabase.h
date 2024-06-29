#pragma once
#include <string>
#include <memory>
#include "CSQLiteDriver.h"
#include "CSystemParamData.h"
#include "CUserData.h"

namespace DBAccess
{

class CDatabase
{
public:
  CDatabase();
  virtual ~CDatabase() = default;

  void OpenDatabase( const std::string& dbFilename );
  void Close( );

  CSystemParamData& GetSystemParamData() {
    return m_sysParamData;
  }

  CUserData& GetUserData() {
    return m_userData;
  }

private:
  CSQLiteDriver m_dbDriver;

  CSystemParamData m_sysParamData;

  CUserData m_userData;
};

}