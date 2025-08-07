#pragma once
#include "IUserData.h"

namespace DBAccess
{
class IDBDriver;

class CUserData : public IUserData
{
public:
  CUserData( IDBDriver& rDBDriver );
  virtual ~CUserData() = default;

  bool AddUser( const knocknock::CUser& user ) override;
  bool UpdateUser( const knocknock::CUser& user ) override;
  void DeleteUser( const std::string& userId ) override;
  std::optional<knocknock::CUser> GetUserByUserId( const std::string& userId) override;
  knocknock::tUserArray GetAllUsers() override;

private:
  IDBDriver& m_rDBDriver;
};

}