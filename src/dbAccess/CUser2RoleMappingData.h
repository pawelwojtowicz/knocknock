#pragma once 
#include "IUser2RoleMappingData.h"

namespace DBAccess
{
class IDBDriver;

class CUser2RoleMappingData : public IUser2RoleMappingData
{
public:
  CUser2RoleMappingData(IDBDriver& rDBDriver);
  ~CUser2RoleMappingData() = default;

  bool AssignRoleToUser( const std::string& userId, const std::string& roleName) override;
  bool RemoveRoleFromUser(const std::string& userId, const std::string& roleName) override;

  knocknock::tRoles GetUserRoles( const std::string& userId) override;
  knocknock::tPrivilegeArray GetUserPrivileges( const std::string& userId) override;

private:
  IDBDriver& m_rDBDriver;

};

}