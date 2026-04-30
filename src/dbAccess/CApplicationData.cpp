#include "CApplicationData.h"
#include "IDBDriver.h"
#include <iostream>

namespace DBAccess
{
CApplicationData::CApplicationData(IDBDriver& rDBDriver)
: m_rDBDriver(rDBDriver)
{
}

bool CApplicationData::AddApplication( const knocknock::CApplication& applicationRecord)
{
  const std::string sql = "INSERT INTO APPLICATIONS (APP_NAME, APP_DATA_PUBLISHER, APP_ACCESS_TOKEN) VALUES (?, ?, ?);";
  std::vector<std::string> params = {
    applicationRecord.GetName(),
    applicationRecord.GetDataPublisher(),
    applicationRecord.GetAccessToken()
  };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

bool CApplicationData::UpdateApplication( const knocknock::CApplication& applicationRecord )
{
  const std::string sql = "UPDATE APPLICATIONS SET APP_NAME=?, APP_DATA_PUBLISHER=?, APP_ACCESS_TOKEN=? WHERE APP_ID=?;";
  std::vector<std::string> params = {
    applicationRecord.GetName(),
    applicationRecord.GetDataPublisher(),
    applicationRecord.GetAccessToken(),
    std::to_string(applicationRecord.GetAppId())
  };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

bool CApplicationData::DeleteApplication( const int appId )
{
  const std::string sql = "DELETE FROM APPLICATIONS WHERE APP_ID=?;";
  std::vector<std::string> params = { std::to_string(appId) };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

std::optional<knocknock::CApplication> CApplicationData::GetApplication( const int appId)
{
  knocknock::CApplication application;

  const std::string sql = "SELECT APP_ID, APP_NAME, APP_DATA_PUBLISHER, APP_ACCESS_TOKEN FROM APPLICATIONS WHERE APP_ID=?;";
  std::vector<std::string> params = { std::to_string(appId) };

  auto getAppCallback = [](void *data, int argc, char **argv, char **azColName) {
    if ( 4 == argc )
    { 
      knocknock::CApplication* pApplication = (knocknock::CApplication*)data;
      *pApplication = knocknock::CApplication(atoi(argv[0]), argv[1], argv[2], argv[3] );
    }
    return 0;
  };

  if (m_rDBDriver.ExecutePreparedStatement(sql, params, getAppCallback, &application) )
  {
    if ( -1 != application.GetAppId() )
    {
      return std::optional<knocknock::CApplication>(application);
    }
  }
  return std::nullopt;
}

knocknock::tApplicationArray CApplicationData::GetAllApplications()
{
  knocknock::tApplicationArray applicationList = {};

  const std::string sql = "SELECT APP_ID, APP_NAME, APP_DATA_PUBLISHER, APP_ACCESS_TOKEN FROM APPLICATIONS;";

  auto getAllAppsCallback = [](void *data, int argc, char **argv, char **azColName) {
    if ( 4 == argc )
    {
      auto appsArray = (knocknock::tApplicationArray*)data;
      appsArray->push_back(knocknock::CApplication( atoi(argv[0]), argv[1], argv[2], argv[3] ) );
    }
    return 0;
  };
  m_rDBDriver.ExecutePreparedStatement(sql, {}, getAllAppsCallback, &applicationList);

  return applicationList;
}  

}