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
  const std::string sqlQuery = "INSERT INTO PRIVILEGES ( SHORT_DESC, LONG_DESC ) VALUES ('"+privilege.GetShortDesc() +"','"+privilege.GetLongDesc()+"');";

  auto insertValueCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, insertValueCallback, 0 ) ;
}

bool CPrivilegeData::UpdatePrivilege(const knocknock::CPrivilege& privilege )
{
  const std::string sqlQuery = "UPDATE PRIVILEGES SET SHORT_DESC='"+privilege.GetShortDesc() +"', LONG_DESC='"+privilege.GetLongDesc()+"' WHERE SHORT_DESC='"+privilege.GetShortDesc() +"';"; 
  auto updateValueCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, updateValueCallback, 0 ) ;
}

bool CPrivilegeData::DeletePrivilege( const std::string& shortDesc)
{
  const std::string sqlQuery = "DELETE FROM PRIVILEGES WHERE SHORT_DESC='"+shortDesc+"';";

  auto deleteCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, deleteCallback, 0 ) ;
}

std::optional<knocknock::CPrivilege> CPrivilegeData::GetPrivilege( const std::string& shortDesc)
{
  const std::string sqlQuery = "SELECT SHORT_DESC, LONG_DESC FROM PRIVILEGES WHERE SHORT_DESC='"+shortDesc+"';";

  knocknock::CPrivilege privilege;

  auto getPrivilegeCallback = [](void *data, int argc, char **argv, char **azColName) {
    knocknock::CPrivilege* pPrivilege = (knocknock::CPrivilege*)data;
    if ( 2 == argc )
    {
      *pPrivilege = knocknock::CPrivilege( argv[0], argv[1]);
    } 
    return 0;
  };

  if ( m_rDBDriver.ExecuteSQLCommand( sqlQuery, getPrivilegeCallback, &privilege ) )
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
  const std::string sqlQuery = "SELECT SHORT_DESC,LONG_DESC FROM PRIVILEGES;";

  auto getAllPrivilegesCallback = [](void *data, int argc, char **argv, char **azColName) {
    if ( 2 == argc )
    {
      auto privilegeArray = (knocknock::tPrivilegeArray*)data;
      privilegeArray->push_back(knocknock::CPrivilege( argv[0], argv[1] ));
    }
    return 0;
  };
  m_rDBDriver.ExecuteSQLCommand( sqlQuery, getAllPrivilegesCallback, &privileges );

  return privileges;
}

}