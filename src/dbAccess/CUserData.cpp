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
  const std::string sql = "INSERT INTO USERS (USER_ID, FIRST_NAME, LAST_NAME, AUTH_METHOD, AUTH_STRING) VALUES (?, ?, ?, ?, ?);";
  std::vector<std::string> params = {
    user.GetUserId(),
    user.GetFirstName(),
    user.GetLastName(),
    user.GetAuthenticationMethod(),
    user.GetAuthenticationString()
  };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

bool CUserData::UpdateUser( const knocknock::CUser& user )
{
  const std::string sql = "UPDATE USERS SET FIRST_NAME=?, LAST_NAME=?, AUTH_METHOD=?, AUTH_STRING=? WHERE USER_ID=?;";
  std::vector<std::string> params = {
    user.GetFirstName(),
    user.GetLastName(),
    user.GetAuthenticationMethod(),
    user.GetAuthenticationString(),
    user.GetUserId()
  };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

void CUserData::DeleteUser( const std::string& userId )
{
  const std::string sql = "DELETE FROM USERS WHERE USER_ID=?;";
  std::vector<std::string> params = { userId };

  m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

std::optional<knocknock::CUser> CUserData::GetUserByUserId( const std::string& userId)
{
  const std::string sql = "SELECT USER_ID, FIRST_NAME, LAST_NAME, AUTH_METHOD, AUTH_STRING FROM USERS WHERE USER_ID=?;";
  std::vector<std::string> params = { userId };

  knocknock::CUser user;

  auto getUserCallback = [](void *data, int argc, char **argv, char **azColName) {
    knocknock::CUser* pUser = (knocknock::CUser*)data;
    if ( 5 == argc )
    {
      *pUser = knocknock::CUser( argv[0], argv[1], argv[2], argv[3], argv[4] );
    } 
    return 0;
  };

  if ( m_rDBDriver.ExecutePreparedStatement(sql, params, getUserCallback, &user) )
  {
    if ( !user.GetUserId().empty() )
    {
      return std::optional<knocknock::CUser>(user);
    }
  } 

  return std::nullopt ;
}

knocknock::tUserArray CUserData::GetAllUsers()
{
  knocknock::tUserArray users = {};
  const std::string sql = "SELECT USER_ID, FIRST_NAME, LAST_NAME, AUTH_METHOD, AUTH_STRING FROM USERS;";

  auto getAllUsersCallback = [](void *data, int argc, char **argv, char **azColName) {
    if ( 5 == argc )
    {
      auto usersArray = (knocknock::tUserArray*)data;
      usersArray->push_back(knocknock::CUser( argv[0], argv[1], argv[2], argv[3], argv[4] ));
    }
    return 0;
  };
  m_rDBDriver.ExecutePreparedStatement(sql, {}, getAllUsersCallback, &users);

  return users;

}

  
}