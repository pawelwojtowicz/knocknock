#pragma once
#include <CRole.h>
#include <optional>

namespace DBAccess
{
class IDBDriver;

class CRoleData
{
public:
  CRoleData( IDBDriver& dbDriver);
  ~CRoleData() = default;

  bool AddRole( const knocknock::CRole& role);
  bool UpdateRole( const knocknock::CRole& role);
  bool DeleteRole( const std::string& name);
  std::optional<knocknock::CRole> GetRole( const std::string& role);
  knocknock::tRoles GetAllRoles();

private:
  IDBDriver& m_rDBDriver;
};
}