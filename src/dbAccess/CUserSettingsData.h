#pragma once
#include <CUserSetting.h>


namespace DBAccess
{
class IDBDriver;

class CUserSettingsData
{
public:
  CUserSettingsData(IDBDriver& rDBDriver);
  ~CUserSettingsData() = default;

  bool AddUserSetting( const knocknock::CUserSetting& userSetting);
  bool UpdateUserSetting(const knocknock::CUserSetting& userSetting);
  bool DeleteUserSetting( const int appId, const std::string& paramName, const std::string& userId );

  knocknock::tUserSettingsArray GetUserSettings( const int  appId, const std::string& userId);
private:
  IDBDriver& m_rDBDriver;
};


}