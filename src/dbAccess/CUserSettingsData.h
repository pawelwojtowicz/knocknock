#pragma once
#include "IUserSettingsData.h"


namespace DBAccess
{
class IDBDriver;

class CUserSettingsData : public IUserSettingsData
{
public:
  CUserSettingsData(IDBDriver& rDBDriver);
  ~CUserSettingsData() = default;

  bool AddUserSetting( const knocknock::CUserSetting& userSetting) override;
  bool UpdateUserSetting(const knocknock::CUserSetting& userSetting) override;
  bool DeleteUserSetting( const int appId, const std::string& paramName, const std::string& userId ) override;

  knocknock::tUserSettingsArray GetUserSettings( const int  appId, const std::string& userId) override;
private:
  IDBDriver& m_rDBDriver;
};


}