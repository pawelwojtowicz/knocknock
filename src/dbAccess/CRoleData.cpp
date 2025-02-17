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
  const std::string sqlQuery = "INSERT INTO ROLES ( NAME, DESCRIPTION ) VALUES ('"+role.GetName() +"','"+role.GetDescription()+"');";

  auto insertValueCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, insertValueCallback, 0 ) ;
}

bool CRoleData::UpdateRole( const knocknock::CRole& role)
{
  const std::string sqlQuery = "UPDATE ROLES SET NAME='"+role.GetName() +"', DESCRIPTION='"+role.GetDescription()+"' WHERE NAME='"+role.GetName() +"';"; 
  auto updateValueCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, updateValueCallback, 0 ) ;

}

bool CRoleData::DeleteRole( const std::string& name)
{
  const std::string sqlQuery = "DELETE FROM ROLES WHERE NAME='"+name+"';";

  auto deleteCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, deleteCallback, 0 ) ;
}
  
std::optional<knocknock::CRole> CRoleData::GetRole( const std::string& roleName)
{
  const std::string sqlQuery = "SELECT NAME, DESCRIPTION FROM ROLES WHERE NAME='"+roleName+"';";

  knocknock::CRole role;

  auto getUserCallback = [](void *data, int argc, char **argv, char **azColName) {
    knocknock::CRole* pRole = (knocknock::CRole*)data;
    if ( 2 == argc )
    {
      *pRole = knocknock::CRole( argv[0], argv[1]);
    } 
    return 0;
  };

  if ( m_rDBDriver.ExecuteSQLCommand( sqlQuery, getUserCallback, &role ) )
  {
    if ( !role.GetName().empty() )
    {
      return std::optional<knocknock::CRole>(role);
    }
  } 

  return std::nullopt ;
}

knocknock::tRoles CRoleData::GetAllRoles()
{
  knocknock::tRoles roles = {};
  const std::string sqlQuery = "SELECT NAME,DESCRIPTION FROM ROLES;";

  auto getAllRolesCallback = [](void *data, int argc, char **argv, char **azColName) {
    if ( 2 == argc )
    {
      auto rolesArray = (knocknock::tRoles*)data;
      rolesArray->push_back(knocknock::CRole( argv[0], argv[1] ));
    }
    return 0;
  };
  m_rDBDriver.ExecuteSQLCommand( sqlQuery, getAllRolesCallback, &roles );

  return roles;


}

}