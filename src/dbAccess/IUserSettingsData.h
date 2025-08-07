#pragma once
#include <CUserSetting.h>

namespace DBAccess
{

class IUserSettingsData
{
public:
    virtual ~IUserSettingsData() = default;

    // CRUD operations for user settings
  virtual bool AddUserSetting( const knocknock::CUserSetting& userSetting) = 0;
  virtual bool UpdateUserSetting(const knocknock::CUserSetting& userSetting) = 0;
  virtual bool DeleteUserSetting( const int appId, const std::string& paramName, const std::string& userId ) = 0;

  virtual knocknock::tUserSettingsArray GetUserSettings( const int  appId, const std::string& userId) = 0;
};

}
