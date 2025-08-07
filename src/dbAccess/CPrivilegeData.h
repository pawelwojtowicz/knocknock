#pragma once
#include "IPrivilegeData.h"

namespace DBAccess
{
class IDBDriver;

class CPrivilegeData : public IPrivilegeData

{
public:
  CPrivilegeData( IDBDriver& dbDriver);
  ~CPrivilegeData() = default;

  bool AddPrivilege( const knocknock::CPrivilege& privilege) override;
  bool UpdatePrivilege(const knocknock::CPrivilege& privilege) override;
  bool DeletePrivilege( const std::string& shortDesc) override;
  std::optional<knocknock::CPrivilege> GetPrivilege( const std::string& shortDesc) override;
  knocknock::tPrivilegeArray GetAllPrivileges() override;

private:
  IDBDriver& m_rDBDriver;
};
}