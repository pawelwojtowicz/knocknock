#pragma once
#include <string>
#include <memory>
#include "CSQLiteDriver.h"
#include "CPrivilegeData.h"
#include "CRoleData.h"
#include "CRole2PrivilegeMappingData.h"
#include "CSystemParamData.h"
#include "CUserData.h"
#include "CUser2RoleMappingData.h"

namespace DBAccess
{

class CDatabase
{
public:
  CDatabase();
  virtual ~CDatabase() = default;

  void OpenDatabase( const std::string& dbFilename );
  void Close( );

  CPrivilegeData& GetPrivilegeData() {
    return m_privilegeData;
  }

  CRoleData& GetRoleData() {
    return m_roleData;
  }

  CRole2PrivilegeMappingData GetRole2PrivilegeMappingData()
  {
    return m_role2PrivilegeMappingData;
  }

  CSystemParamData& GetSystemParamData() {
    return m_sysParamData;
  }

  CUserData& GetUserData() {
    return m_userData;
  }

  CUser2RoleMappingData GetUser2RoleMappingData()
  {
    return m_user2RoleMappingData;
  }

private:
  CSQLiteDriver m_dbDriver;

  CPrivilegeData m_privilegeData;

  CRoleData m_roleData;

  CRole2PrivilegeMappingData m_role2PrivilegeMappingData;

  CSystemParamData m_sysParamData;

  CUserData m_userData;

  CUser2RoleMappingData m_user2RoleMappingData;
};

}