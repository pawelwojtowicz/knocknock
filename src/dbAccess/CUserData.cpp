#include "CUserData.h"
#include "IDBDriver.h"
#include <iostream>

namespace DBAccess
{

CUserData::CUserData( IDBDriver& rDBDriver )
: m_rDBDriver(rDBDriver)
{
}

bool CUserData::AddUser( const knocknock::CUser& user )
{
  const std::string sqlQuery = "INSERT INTO USERS (USER_ID, FIRST_NAME , LAST_NAME , PASS_HASH) VALUES ('"+user.getUserId() +"','"+user.getFirstName()+"','"+user.getLastName()+"','"+user.getPasswordHash()+"');";

  std::cout << sqlQuery << std::endl;

  auto insertValueCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  int rc = m_rDBDriver.ExecuteSQLCommand( sqlQuery, insertValueCallback, 0 );

  return ( 0 != rc ) ;
}

bool CUserData::UpdateUser( const knocknock::CUser& user )
{
  return true;
}

bool CUserData::DeleteUser( const std::string& userId )
{
  return true;
}

std::optional<knocknock::CUser> CUserData::GetUserByUserId( const std::string& userId)
{
  return std::nullopt;
}

knocknock::tUserArray CUserData::GetAllUsers()
{
  return {};
}

  
}