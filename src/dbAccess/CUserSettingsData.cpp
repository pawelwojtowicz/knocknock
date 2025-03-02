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
  const std::string sqlQuery = "INSERT INTO USER_SETTINGS ( APPLICATION_ID, APPLICATION_PARAM_NAME, USER_ID, VALUE ) VALUES ( '"+std::to_string(userSetting.GetApplicationId()) +"','"+userSetting.GetParamName()+"','"+userSetting.GetUserId()+"','"+userSetting.GetParamValue()+"');";

  auto insertValueCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, insertValueCallback, 0 ) ;
}

bool CUserSettingsData::UpdateUserSetting(const knocknock::CUserSetting& userSetting)
{
  const std::string sqlQuery = "UPDATE USER_SETTINGS SET VALUE='"+userSetting.GetParamValue()+"' WHERE APPLICATION_ID="+std::to_string(userSetting.GetApplicationId())+" AND APPLICATION_PARAM_NAME='"+userSetting.GetParamName()+"' AND USER_ID='"+userSetting.GetUserId()+"';"; 
  auto updateCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };
  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, updateCallback, 0 ) ;
}

bool CUserSettingsData::DeleteUserSetting( const int appId, const std::string& paramName, const std::string& userId )
{
  std::string deleteParamQuery = "DELETE FROM USER_SETTINGS WHERE APPLICATION_ID="+std::to_string(appId)+" AND APPLICATION_PARAM_NAME='"+paramName+"' AND USER_ID='"+userId+"';";

  auto deleteCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };
  return m_rDBDriver.ExecuteSQLCommand( deleteParamQuery, deleteCallback, 0 );
}

knocknock::tUserSettingsArray CUserSettingsData::GetUserSettings( const int  appId, const std::string& userId)
{
  knocknock::tUserSettingsArray userSettings = {} ;

  std::string getUserSettingsQuery = "SELECT APPLICATION_ID, APPLICATION_PARAM_NAME, USER_ID, VALUE FROM USER_SETTINGS WHERE APPLICATION_ID="+std::to_string(appId)+" AND USER_ID='"+userId+"';";
  
  
  auto getUserSettingsCallback = [](void *data, int argc, char **argv, char **azColName) {
    if (argc == 4 )
    {
      knocknock::tUserSettingsArray* pUserSettings = (knocknock::tUserSettingsArray*)data;
      pUserSettings->push_back(knocknock::CUserSetting(atoi(argv[0]), argv[1], argv[2], argv[3]));
    }
    return 0;
  };

  m_rDBDriver.ExecuteSQLCommand( getUserSettingsQuery, getUserSettingsCallback, &userSettings );
  return userSettings;
}


}