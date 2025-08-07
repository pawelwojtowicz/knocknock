#pragma once
#include "IRole2PrivilegeMappingData.h"

namespace DBAccess
{
class IDBDriver;

class CRole2PrivilegeMappingData : public IRole2PrivilegeMappingData
{
public:
  CRole2PrivilegeMappingData( IDBDriver& rDBDriver);
  ~CRole2PrivilegeMappingData() = default;

  bool AddPrivilege2Role( const std::string& roleName, const std::string& privileShortDesc) override;
  bool RemovePrivilegeFromRole( const std::string& roleName, const std::string& privileShortDesc) override;

  knocknock::tPrivilegeArray GetPrivilegesForRole( const std::string& roleName) override;

private:
  IDBDriver& m_rDBDriver;
};
}