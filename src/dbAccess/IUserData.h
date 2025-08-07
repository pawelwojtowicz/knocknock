#pragma once
#include <CUser.h>
#include <optional>


namespace DBAccess
{

class IUserData
{
public:
    virtual ~IUserData() = default;

    // CRUD operations
  virtual bool AddUser( const knocknock::CUser& user ) = 0;
  virtual bool UpdateUser( const knocknock::CUser& user ) = 0;
  virtual void DeleteUser( const std::string& userId ) = 0;
  virtual std::optional<knocknock::CUser> GetUserByUserId( const std::string& userId) = 0;
  virtual knocknock::tUserArray GetAllUsers() = 0;
};

}
