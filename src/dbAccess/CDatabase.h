#pragma once
#include <string>
#include "IDBAccess.h"
#include "IDBDriver.h"
#include "CApplicationData.h"
#include "CApplicationParamData.h"
#include "CPrivilegeData.h"
#include "CRoleData.h"
#include "CRole2PrivilegeMappingData.h"
#include "CSystemParamData.h"
#include "CUserData.h"
#include "CUser2RoleMappingData.h"
#include "CUserSettingsData.h"

namespace DBAccess
{

class CDatabase : public IDBAccess
{
public:
  CDatabase( IDBDriver& rDBDriver );
  virtual ~CDatabase() = default;

  void OpenDatabase( const std::string& dbFilename );
  void Close( );

  CApplicationData& GetApplicationData() override {
    return m_applicationData;
  }

  IApplicationParamData& GetApplicationParamData() override {
    return m_applicationParamData;
  }

  IPrivilegeData& GetPrivilegeData() override {
    return m_privilegeData;
  }

  IRoleData& GetRoleData() override {
    return m_roleData;
  }

  IRole2PrivilegeMappingData& GetRole2PrivilegeMappingData() override
  {
    return m_role2PrivilegeMappingData;
  }

  ISystemParamData& GetSystemParamData() override {
    return m_sysParamData;
  }

  IUserData& GetUserData() override {
    return m_userData;
  }

  IUser2RoleMappingData& GetUser2RoleMappingData() override
  {
    return m_user2RoleMappingData;
  }

  IUserSettingsData& GetUserSettingsData() override
  {
    return m_userSettingsData;
  }

private:
  IDBDriver& m_rDBDriver;

  CApplicationData m_applicationData;

  CApplicationParamData m_applicationParamData;

  CPrivilegeData m_privilegeData;

  CRoleData m_roleData;

  CRole2PrivilegeMappingData m_role2PrivilegeMappingData;

  CSystemParamData m_sysParamData;

  CUserData m_userData;

  CUser2RoleMappingData m_user2RoleMappingData;

  CUserSettingsData m_userSettingsData;
};

}