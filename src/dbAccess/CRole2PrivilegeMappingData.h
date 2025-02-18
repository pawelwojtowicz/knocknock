#pragma once
#include <string>
#include <CPrivilege.h>

namespace DBAccess
{
class IDBDriver;

class CRole2PrivilegeMappingData
{
public:
  CRole2PrivilegeMappingData( IDBDriver& rDBDriver);
  ~CRole2PrivilegeMappingData() = default;

  bool AddPrivilege2Role( const std::string& roleName, const std::string& privileShortDesc);
  bool RemovePrivilegeFromRole( const std::string& roleName, const std::string& privileShortDesc );

  knocknock::tPrivilegeArray GetPrivilegesForRole( const std::string& roleName);

private:
  IDBDriver& m_rDBDriver;
};
}