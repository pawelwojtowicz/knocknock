#include "CUserData.h"

namespace DBAccess
{

CUserData::CUserData( IDBDriver& rDBDriver )
: m_rDBDriver(rDBDriver)
{
}

bool CUserData::AddUser( const knocknock::CUser& user )
{
  return true;
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