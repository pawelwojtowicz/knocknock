#include "CRoleData.h"
#include "IDBDriver.h"

namespace DBAccess
{
CRoleData::CRoleData( IDBDriver& dbDriver)
: m_rDBDriver(dbDriver)
{
}

bool CRoleData::AddRole( const knocknock::CRole& role)
{
  const std::string sql = "INSERT INTO ROLES (NAME, DESCRIPTION) VALUES (?, ?);";
  std::vector<std::string> params = { role.GetName(), role.GetDescription() };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

bool CRoleData::UpdateRole( const knocknock::CRole& role)
{
  const std::string sql = "UPDATE ROLES SET DESCRIPTION=? WHERE NAME=?;";
  std::vector<std::string> params = { role.GetDescription(), role.GetName() };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

bool CRoleData::DeleteRole( const std::string& name)
{
  const std::string sql = "DELETE FROM ROLES WHERE NAME=?;";
  std::vector<std::string> params = { name };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}
  
std::optional<knocknock::CRole> CRoleData::GetRole( const std::string& roleName) const
{
  const std::string sql = "SELECT NAME, DESCRIPTION FROM ROLES WHERE NAME=?;";
  std::vector<std::string> params = { roleName };

  knocknock::CRole role;

  auto getUserCallback = [](void *data, int argc, char **argv, char **azColName) {
    knocknock::CRole* pRole = (knocknock::CRole*)data;
    if ( 2 == argc )
    {
      *pRole = knocknock::CRole( argv[0], argv[1]);
    } 
    return 0;
  };

  if ( m_rDBDriver.ExecutePreparedStatement(sql, params, getUserCallback, &role) )
  {
    if ( !role.GetName().empty() )
    {
      return std::optional<knocknock::CRole>(role);
    }
  } 

  return std::nullopt ;
}

knocknock::tRoles CRoleData::GetAllRoles() const
{
  knocknock::tRoles roles = {};
  const std::string sql = "SELECT NAME, DESCRIPTION FROM ROLES;";

  auto getAllRolesCallback = [](void *data, int argc, char **argv, char **azColName) {
    if ( 2 == argc )
    {
      auto rolesArray = (knocknock::tRoles*)data;
      rolesArray->push_back(knocknock::CRole( argv[0], argv[1] ));
    }
    return 0;
  };
  m_rDBDriver.ExecutePreparedStatement(sql, {}, getAllRolesCallback, &roles);

  return roles;
}

}