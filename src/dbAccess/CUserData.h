#pragma once
#include <CUser.h>
#include <optional>

namespace DBAccess
{
class IDBDriver;

class CUserData
{
public:
  CUserData( IDBDriver& rDBDriver );
  virtual ~CUserData() = default;

  bool AddUser( const knocknock::CUser& user );
  bool UpdateUser( const knocknock::CUser& user );
  bool DeleteUser( const std::string& userId );
  std::optional<knocknock::CUser> GetUserByUserId( const std::string& userId);
  knocknock::tUserArray GetAllUsers();

private:
  IDBDriver& m_rDBDriver;
};

}