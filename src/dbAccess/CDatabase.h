#pragma once
#include <string>
#include <memory>
#include "CSQLiteDriver.h"
#include "CRoleData.h"
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

  CRoleData& GetRoleData() {
    return m_roleData;
  }

  CSystemParamData& GetSystemParamData() {
    return m_sysParamData;
  }

  CUserData& GetUserData() {
    return m_userData;
  }

private:
  CSQLiteDriver m_dbDriver;

  CRoleData m_roleData;

  CSystemParamData m_sysParamData;


  CUserData m_userData;
};

}