#include "CPrivilegeData.h"
#include "IDBDriver.h"
#include "CPrivilege.h"

namespace DBAccess
{

CPrivilegeData::CPrivilegeData( IDBDriver& dbDriver)
: m_rDBDriver(dbDriver)
{
}

bool CPrivilegeData::AddPrivilege( const knocknock::CPrivilege& privilege)
{
  const std::string sql = "INSERT INTO PRIVILEGES (SHORT_DESC, LONG_DESC) VALUES (?, ?);";
  std::vector<std::string> params = { privilege.GetShortDesc(), privilege.GetLongDesc() };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

bool CPrivilegeData::UpdatePrivilege(const knocknock::CPrivilege& privilege )
{
  const std::string sql = "UPDATE PRIVILEGES SET LONG_DESC=? WHERE SHORT_DESC=?;";
  std::vector<std::string> params = { privilege.GetLongDesc(), privilege.GetShortDesc() };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

bool CPrivilegeData::DeletePrivilege( const std::string& shortDesc)
{
  const std::string sql = "DELETE FROM PRIVILEGES WHERE SHORT_DESC=?;";
  std::vector<std::string> params = { shortDesc };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

std::optional<knocknock::CPrivilege> CPrivilegeData::GetPrivilege( const std::string& shortDesc)
{
  const std::string sql = "SELECT SHORT_DESC, LONG_DESC FROM PRIVILEGES WHERE SHORT_DESC=?;";
  std::vector<std::string> params = { shortDesc };

  knocknock::CPrivilege privilege;

  auto getPrivilegeCallback = [](void *data, int argc, char **argv, char **azColName) {
    knocknock::CPrivilege* pPrivilege = (knocknock::CPrivilege*)data;
    if ( 2 == argc )
    {
      *pPrivilege = knocknock::CPrivilege( argv[0], argv[1]);
    } 
    return 0;
  };

  if ( m_rDBDriver.ExecutePreparedStatement(sql, params, getPrivilegeCallback, &privilege) )
  {
    if ( !privilege.GetShortDesc().empty() )
    {
      return std::optional<knocknock::CPrivilege>(privilege);
    }
  } 

  return std::nullopt ;
}

knocknock::tPrivilegeArray CPrivilegeData::GetAllPrivileges()
{
  knocknock::tPrivilegeArray privileges = {};
  const std::string sql = "SELECT SHORT_DESC, LONG_DESC FROM PRIVILEGES;";

  auto getAllPrivilegesCallback = [](void *data, int argc, char **argv, char **azColName) {
    if ( 2 == argc )
    {
      auto privilegeArray = (knocknock::tPrivilegeArray*)data;
      privilegeArray->push_back(knocknock::CPrivilege( argv[0], argv[1] ));
    }
    return 0;
  };
  m_rDBDriver.ExecutePreparedStatement(sql, {}, getAllPrivilegesCallback, &privileges);

  return privileges;
}

}