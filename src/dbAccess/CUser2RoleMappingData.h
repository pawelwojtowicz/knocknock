#pragma once 
#include <string>
#include <CRole.h>
#include <CPrivilege.h>

namespace DBAccess
{
class IDBDriver;

class CUser2RoleMappingData
{
public:
  CUser2RoleMappingData(IDBDriver& rDBDriver);
  ~CUser2RoleMappingData() = default;

  bool AssignRoleToUser( const std::string& userId, const std::string& roleName);
  bool RemoveRoleFromUser(const std::string& userId, const std::string& roleName);

  knocknock::tRoles GetUserRoles( const std::string& userId);
  knocknock::tPrivilegeArray GetUserPrivileges( const std::string& userId);

private:
  IDBDriver& m_rDBDriver;

};

}