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
  const std::string sqlQuery = "INSERT INTO APPLICATIONS ( APP_NAME, APP_DATA_PUBLISHER, APP_ACCESS_TOKEN ) VALUES ( '"+applicationRecord.GetName() +"','"+applicationRecord.GetDataPublisher()+"','"+applicationRecord.GetAccessToken()+"');";

  auto insertValueCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, insertValueCallback, 0 ) ;
}

bool CApplicationData::UpdateApplication( const knocknock::CApplication& applicationRecord )
{
  const std::string sqlQuery = "UPDATE APPLICATIONS SET APP_NAME='"+applicationRecord.GetName() +"', APP_DATA_PUBLISHER='"+applicationRecord.GetDataPublisher()+"', APP_ACCESS_TOKEN='"+applicationRecord.GetAccessToken()+"' WHERE APP_ID="+std::to_string(applicationRecord.GetAppId())+";"; 
  auto updateValueCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, updateValueCallback, 0 ) ;
}

bool CApplicationData::DeleteApplication( const int appId )
{
  const std::string sqlQuery = "DELETE FROM APPLICATIONS WHERE APP_ID="+std::to_string(appId)+";";

  auto deleteCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, deleteCallback, 0 ) ;
}

std::optional<knocknock::CApplication> CApplicationData::GetApplication( const int appId)
{
  knocknock::CApplication application;

  const std::string sqlQuery = "SELECT APP_ID, APP_NAME, APP_DATA_PUBLISHER, APP_ACCESS_TOKEN FROM APPLICATIONS WHERE APP_ID="+std::to_string(appId)+";";

  auto getAppCallback = [](void *data, int argc, char **argv, char **azColName) {
    if ( 4 == argc )
    { 
      knocknock::CApplication* pApplication = (knocknock::CApplication*)data;
      *pApplication = knocknock::CApplication(atoi(argv[0]), argv[1], argv[2], argv[3] );
    }
    return 0;
  };

  if (m_rDBDriver.ExecuteSQLCommand( sqlQuery, getAppCallback, &application ) )
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

  const std::string sqlQuery = "SELECT APP_ID, APP_NAME, APP_DATA_PUBLISHER, APP_ACCESS_TOKEN FROM APPLICATIONS;";

  auto getAllAppsCallback = [](void *data, int argc, char **argv, char **azColName) {
    if ( 4 == argc )
    {
      auto appsArray = (knocknock::tApplicationArray*)data;
      appsArray->push_back(knocknock::CApplication( atoi(argv[0]), argv[1], argv[2], argv[3] ) );
    }
    return 0;
  };
  m_rDBDriver.ExecuteSQLCommand( sqlQuery, getAllAppsCallback, &applicationList );

  return applicationList;
}  

}