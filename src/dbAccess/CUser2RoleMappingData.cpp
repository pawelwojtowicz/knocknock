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
  const std::string sqlQuery = "INSERT INTO USER2ROLES ( USER_ID, ROLE_NAME ) VALUES ('"+ userId +"','"+roleName+"');";

  auto insertValueCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, insertValueCallback, 0 ) ;
}

bool CUser2RoleMappingData::RemoveRoleFromUser(const std::string& userId, const std::string& roleName)
{
  const std::string sqlQuery = "DELETE FROM USER2ROLES WHERE ( USER_ID='"+userId+"' AND ROLE_NAME='"+roleName+"');";

  auto deleteCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, deleteCallback, 0 ) ;
}

knocknock::tRoles CUser2RoleMappingData::GetUserRoles( const std::string& userId)
{
  knocknock::tRoles roles;

  const std::string sqlQuery = "SELECT R.NAME, R.DESCRIPTION FROM USERS U JOIN USER2ROLES U2R ON ( U.USER_ID = U2R.USER_ID ) JOIN ROLES R ON (U2R.ROLE_NAME = R.NAME) WHERE U.USER_ID = '"+userId+"'";

  auto getRolePrivilegesCallback = [](void *data, int argc, char **argv, char **azColName) {
    if ( 2 == argc )
    {
      auto rolesArray = (knocknock::tRoles*)data;
      rolesArray->push_back(knocknock::CRole( argv[0], argv[1] ));
    }
    return 0;
  };
  m_rDBDriver.ExecuteSQLCommand( sqlQuery, getRolePrivilegesCallback, &roles );


  return roles;
}

knocknock::tPrivilegeArray CUser2RoleMappingData::GetUserPrivileges( const std::string& userId)
{
  knocknock::tPrivilegeArray privileges;

  const std::string sqlQuery = "SELECT P.SHORT_DESC, P.LONG_DESC FROM USERS U JOIN USER2ROLES U2R ON ( U.USER_ID = U2R.USER_ID ) JOIN ROLE2PRIVILEGE R2P ON ( U2R.ROLE_NAME = R2P.ROLE_NAME ) JOIN PRIVILEGES P ON (R2P.PRIVILEGE_SHORT_DESC = P.SHORT_DESC)  WHERE U.USER_ID= '"+userId+"'";

  auto getUserPrivilegesCallback = [](void *data, int argc, char **argv, char **azColName) {
    if ( 2 == argc )
    {
      auto privilegeArray = (knocknock::tPrivilegeArray*)data;
      privilegeArray->push_back(knocknock::CPrivilege( argv[0], argv[1] ));
    }
    return 0;
  };
  m_rDBDriver.ExecuteSQLCommand( sqlQuery, getUserPrivilegesCallback, &privileges );


  return privileges;
}

  
}