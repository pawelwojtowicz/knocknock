#include "CUser2RoleMappingData.h"
#include "IDBDriver.h"

namespace DBAccess
{
CUser2RoleMappingData::CUser2RoleMappingData(IDBDriver& rDBDriver)
: m_rDBDriver(rDBDriver)
{  
}

bool CUser2RoleMappingData::AssignRoleToUser( const std::string& userId, const std::string& roleName)
{
  const std::string sql = "INSERT INTO USER2ROLES (USER_ID, ROLE_NAME) VALUES (?, ?);";
  std::vector<std::string> params = { userId, roleName };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

bool CUser2RoleMappingData::RemoveRoleFromUser(const std::string& userId, const std::string& roleName)
{
  const std::string sql = "DELETE FROM USER2ROLES WHERE USER_ID=? AND ROLE_NAME=?;";
  std::vector<std::string> params = { userId, roleName };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

knocknock::tRoles CUser2RoleMappingData::GetUserRoles( const std::string& userId)
{
  knocknock::tRoles roles;

  const std::string sql = "SELECT R.NAME, R.DESCRIPTION FROM USERS U JOIN USER2ROLES U2R ON (U.USER_ID = U2R.USER_ID) JOIN ROLES R ON (U2R.ROLE_NAME = R.NAME) WHERE U.USER_ID = ?;";
  std::vector<std::string> params = { userId };

  auto getRolePrivilegesCallback = [](void *data, int argc, char **argv, char **azColName) {
    if ( 2 == argc )
    {
      auto rolesArray = (knocknock::tRoles*)data;
      rolesArray->push_back(knocknock::CRole( argv[0], argv[1] ));
    }
    return 0;
  };
  m_rDBDriver.ExecutePreparedStatement(sql, params, getRolePrivilegesCallback, &roles);

  return roles;
}

knocknock::tPrivilegeArray CUser2RoleMappingData::GetUserPrivileges( const std::string& userId)
{
  knocknock::tPrivilegeArray privileges;

  const std::string sql = "SELECT P.SHORT_DESC, P.LONG_DESC FROM USERS U JOIN USER2ROLES U2R ON (U.USER_ID = U2R.USER_ID) JOIN ROLE2PRIVILEGE R2P ON (U2R.ROLE_NAME = R2P.ROLE_NAME) JOIN PRIVILEGES P ON (R2P.PRIVILEGE_SHORT_DESC = P.SHORT_DESC) WHERE U.USER_ID = ?;";
  std::vector<std::string> params = { userId };

  auto getUserPrivilegesCallback = [](void *data, int argc, char **argv, char **azColName) {
    if ( 2 == argc )
    {
      auto privilegeArray = (knocknock::tPrivilegeArray*)data;
      privilegeArray->push_back(knocknock::CPrivilege( argv[0], argv[1] ));
    }
    return 0;
  };
  m_rDBDriver.ExecutePreparedStatement(sql, params, getUserPrivilegesCallback, &privileges);

  return privileges;
}

  
}