#pragma once
#include <CPrivilege.h>
#include <optional>

namespace DBAccess
{
class IDBDriver;

class CPrivilegeData
{
public:
  CPrivilegeData( IDBDriver& dbDriver);
  ~CPrivilegeData() = default;

  bool AddPrivilege( const knocknock::CPrivilege& privilege);
  bool UpdatePrivilege(const knocknock::CPrivilege& privilege );
  bool DeletePrivilege( const std::string& shortDesc);
  std::optional<knocknock::CPrivilege> GetPrivilege( const std::string& shortDesc);
  knocknock::tPrivilegeArray GetAllPrivileges();

private:
  IDBDriver& m_rDBDriver;
};
}