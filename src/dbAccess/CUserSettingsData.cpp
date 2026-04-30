#include "CUserSettingsData.h"
#include "IDBDriver.h"

namespace DBAccess
{
CUserSettingsData::CUserSettingsData(IDBDriver& rDBDriver)
: m_rDBDriver(rDBDriver)
{
}

bool CUserSettingsData::AddUserSetting( const knocknock::CUserSetting& userSetting)
{
  const std::string sql = "INSERT INTO USER_SETTINGS (APPLICATION_ID, APPLICATION_PARAM_NAME, USER_ID, VALUE) VALUES (?, ?, ?, ?);";
  std::vector<std::string> params = {
    std::to_string(userSetting.GetApplicationId()),
    userSetting.GetParamName(),
    userSetting.GetUserId(),
    userSetting.GetParamValue()
  };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

bool CUserSettingsData::UpdateUserSetting(const knocknock::CUserSetting& userSetting)
{
  const std::string sql = "UPDATE USER_SETTINGS SET VALUE=? WHERE APPLICATION_ID=? AND APPLICATION_PARAM_NAME=? AND USER_ID=?;";
  std::vector<std::string> params = {
    userSetting.GetParamValue(),
    std::to_string(userSetting.GetApplicationId()),
    userSetting.GetParamName(),
    userSetting.GetUserId()
  };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

bool CUserSettingsData::DeleteUserSetting( const int appId, const std::string& paramName, const std::string& userId )
{
  const std::string sql = "DELETE FROM USER_SETTINGS WHERE APPLICATION_ID=? AND APPLICATION_PARAM_NAME=? AND USER_ID=?;";
  std::vector<std::string> params = { std::to_string(appId), paramName, userId };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

knocknock::tUserSettingsArray CUserSettingsData::GetUserSettings( const int  appId, const std::string& userId)
{
  knocknock::tUserSettingsArray userSettings = {};

  const std::string sql = "SELECT APPLICATION_ID, APPLICATION_PARAM_NAME, USER_ID, VALUE FROM USER_SETTINGS WHERE APPLICATION_ID=? AND USER_ID=?;";
  std::vector<std::string> params = { std::to_string(appId), userId };

  auto getUserSettingsCallback = [](void *data, int argc, char **argv, char **azColName) {
    if (argc == 4)
    {
      knocknock::tUserSettingsArray* pUserSettings = (knocknock::tUserSettingsArray*)data;
      pUserSettings->push_back(knocknock::CUserSetting(atoi(argv[0]), argv[1], argv[2], argv[3]));
    }
    return 0;
  };

  m_rDBDriver.ExecutePreparedStatement(sql, params, getUserSettingsCallback, &userSettings);
  return userSettings;
}


}