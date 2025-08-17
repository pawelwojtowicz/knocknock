#pragma once
#include "IUserData.h"
#include "IRoleData.h"
#include "IPrivilegeData.h"
#include "IUser2RoleMappingData.h"
#include "IRole2PrivilegeMappingData.h"
#include "IApplicationData.h"
#include "IApplicationParamData.h"
#include "IUserSettingsData.h"
#include "ISystemParamData.h"

namespace DBAccess
{

class IDBAccess 
{
public:
  virtual ~IDBAccess() = default;
  
  // Add methods to access different data repositories
  virtual IUserData& GetUserData() = 0;
  virtual IRoleData& GetRoleData() = 0;
  virtual IPrivilegeData& GetPrivilegeData() = 0;
  virtual IUser2RoleMappingData& GetUser2RoleMappingData() = 0;
  virtual IRole2PrivilegeMappingData& GetRole2PrivilegeMappingData() = 0;
  virtual IApplicationData& GetApplicationData() = 0;
  virtual IApplicationParamData& GetApplicationParamData() = 0;
  virtual IUserSettingsData& GetUserSettingsData() = 0;
  virtual ISystemParamData& GetSystemParamData() = 0;

};
}