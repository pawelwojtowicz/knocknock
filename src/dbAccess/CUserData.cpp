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
  const std::string sqlQuery = "INSERT INTO USERS (USER_ID, FIRST_NAME , LAST_NAME , AUTH_METHOD, AUTH_STRING) VALUES ('"+user.getUserId() +"','"+user.getFirstName()+"','"+user.getLastName()+"','"+user.getAuthenticationMethod()+"','"+user.getAuthenticationString()+"');";

  auto insertValueCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, insertValueCallback, 0 ) ;
}

bool CUserData::UpdateUser( const knocknock::CUser& user )
{
  const std::string sqlQuery = "UPDATE USERS SET FIRST_NAME='"+user.getFirstName()+"', LAST_NAME='"+user.getLastName()+"', AUTH_METHOD='"+user.getAuthenticationMethod()+"', AUTH_STRING='"+user.getAuthenticationString()+"' WHERE USER_ID='"+user.getUserId()+"';"; 
  auto updateValueCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, updateValueCallback, 0 ) ;
}

void CUserData::DeleteUser( const std::string& userId )
{
  const std::string sqlQuery = "DELETE FROM USERS WHERE USER_ID='"+userId+"';";

  auto deleteCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  m_rDBDriver.ExecuteSQLCommand( sqlQuery, deleteCallback, 0 ) ;
}

std::optional<knocknock::CUser> CUserData::GetUserByUserId( const std::string& userId)
{
  const std::string sqlQuery = "SELECT USER_ID,FIRST_NAME,LAST_NAME, AUTH_METHOD, AUTH_STRING FROM USERS WHERE USER_ID='"+userId+"';";

  knocknock::CUser user;

  auto getUserCallback = [](void *data, int argc, char **argv, char **azColName) {
    knocknock::CUser* pUser = (knocknock::CUser*)data;
    if ( 5 == argc )
    {
      *pUser = knocknock::CUser( argv[0], argv[1], argv[2], argv[3], argv[4] );
    } 
    return 0;
  };

  if ( m_rDBDriver.ExecuteSQLCommand( sqlQuery, getUserCallback, &user ) )
  {
    if ( !user.getUserId().empty() )
    {
      return std::optional<knocknock::CUser>(user);
    }
  } 

  return std::nullopt ;
}

knocknock::tUserArray CUserData::GetAllUsers()
{
  knocknock::tUserArray users = {};
  const std::string sqlQuery = "SELECT USER_ID,FIRST_NAME,LAST_NAME, AUTH_METHOD, AUTH_STRING FROM USERS;";

  auto getAllUsersCallback = [](void *data, int argc, char **argv, char **azColName) {
    if ( 5 == argc )
    {
      auto usersArray = (knocknock::tUserArray*)data;
      usersArray->push_back(knocknock::CUser( argv[0], argv[1], argv[2], argv[3], argv[4] ));
    }
    return 0;
  };
  m_rDBDriver.ExecuteSQLCommand( sqlQuery, getAllUsersCallback, &users );

  return users;

}

  
}