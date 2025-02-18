#include "CRole2PrivilegeMappingData.h"
#include "IDBDriver.h"

namespace DBAccess
{
 CRole2PrivilegeMappingData::CRole2PrivilegeMappingData( IDBDriver& rDBDriver)
: m_rDBDriver(rDBDriver)
{
}

bool CRole2PrivilegeMappingData::AddPrivilege2Role( const std::string& roleName, const std::string& privilegeShortDesc)
{
  const std::string sqlQuery = "INSERT INTO ROLE2PRIVILEGE ( ROLE_NAME, PRIVILEGE_SHORT_DESC ) VALUES ('"+ roleName +"','"+privilegeShortDesc+"');";

  auto insertValueCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, insertValueCallback, 0 ) ;
}

bool CRole2PrivilegeMappingData::RemovePrivilegeFromRole( const std::string& roleName, const std::string& privileShortDesc )
{
  const std::string sqlQuery = "DELETE FROM ROLE2PRIVILEGE WHERE ( ROLE_NAME='"+roleName+"' AND PRIVILEGE_SHORT_DESC='"+privileShortDesc+"');";

  auto deleteCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, deleteCallback, 0 ) ;
}

knocknock::tPrivilegeArray CRole2PrivilegeMappingData::GetPrivilegesForRole( const std::string& roleName)
{
  knocknock::tPrivilegeArray privileges = {};

  const std::string sqlQuery = "SELECT P.SHORT_DESC, P.LONG_DESC FROM ROLES R JOIN ROLE2PRIVILEGE R2P ON ( R.NAME = R2P.ROLE_NAME ) JOIN PRIVILEGES P ON (R2P.PRIVILEGE_SHORT_DESC = P.SHORT_DESC) WHERE R.NAME = '"+roleName+"'";

  auto getRolePrivilegesCallback = [](void *data, int argc, char **argv, char **azColName) {
    if ( 2 == argc )
    {
      auto privilegeArray = (knocknock::tPrivilegeArray*)data;
      privilegeArray->push_back(knocknock::CPrivilege( argv[0], argv[1] ));
    }
    return 0;
  };
  m_rDBDriver.ExecuteSQLCommand( sqlQuery, getRolePrivilegesCallback, &privileges );

  return privileges;
}

}