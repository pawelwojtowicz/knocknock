#pragma once
#include "IRoleData.h"

namespace DBAccess
{
class IDBDriver;

class CRoleData : public IRoleData
{
public:
  CRoleData( IDBDriver& dbDriver);
  ~CRoleData() = default;

  bool AddRole( const knocknock::CRole& role) override;
  bool UpdateRole( const knocknock::CRole& role) override;
  bool DeleteRole( const std::string& name) override;
  std::optional<knocknock::CRole> GetRole( const std::string& role) const override;
  knocknock::tRoles GetAllRoles() const override;

private:
  IDBDriver& m_rDBDriver;
};
}