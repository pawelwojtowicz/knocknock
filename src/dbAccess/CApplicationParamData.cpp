#include "CApplicationParamData.h"
#include "IDBDriver.h"
#include <iostream>

namespace DBAccess
{

CApplicationParamData::CApplicationParamData( IDBDriver& dbDriver)
: m_rDBDriver(dbDriver)
{
}

bool CApplicationParamData::AddApplicationParam( const knocknock::CApplicationParam& applicationParam )
{
  int isPublic = applicationParam.IsPublic() ? 1 : 0;

  const std::string sqlQuery = "INSERT INTO APPLICATION_PARAMS ( APPLICATION_ID, NAME, PUBLIC, VALUE ) VALUES ( "+std::to_string(applicationParam.GetApplicationId()) 
                      +",'"+applicationParam.GetParameterName()
                      +"',"+ std::to_string(isPublic)
                      +",'"+applicationParam.GetValue()+"');";
  auto insertValueCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, insertValueCallback, 0 ) ;
}

bool CApplicationParamData::UpdateApplicationParam( const knocknock::CApplicationParam& applicationParam )
{
  int isPublic = applicationParam.IsPublic() ? 1 : 0;

  const std::string sqlQuery = "UPDATE APPLICATION_PARAMS SET PUBLIC="+std::to_string(isPublic)+", VALUE='"+applicationParam.GetValue()+"' WHERE APPLICATION_ID="+std::to_string(applicationParam.GetApplicationId())+" AND NAME='"+applicationParam.GetParameterName()+"';"; 
  auto updateCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, updateCallback, 0 ) ;
}

std::optional<knocknock::CApplicationParam> CApplicationParamData::GetApplicationParam( const int applicationId, const std::string& paramName)
{
  std::string getParamQuery = "SELECT APPLICATION_ID, NAME, PUBLIC, VALUE FROM APPLICATION_PARAMS WHERE APPLICATION_ID="+std::to_string(applicationId)+" AND NAME='"+paramName+"';";
  knocknock::CApplicationParam applicationParameter = {} ;

  auto getParamCallback = [](void *data, int argc, char **argv, char **azColName) {
    if (argc == 4 )
    {
      knocknock::CApplicationParam* pAppParam( (knocknock::CApplicationParam*)data);
      *pAppParam = knocknock::CApplicationParam(atoi(argv[0]), argv[1], static_cast<bool>(atoi(argv[2])), argv[3]);
    }
    return 0;
  };

  if ( m_rDBDriver.ExecuteSQLCommand( getParamQuery, getParamCallback, &applicationParameter ) );
  {
    if ( applicationParameter.GetApplicationId() == applicationId )
    {
      return std::optional<knocknock::CApplicationParam>(applicationParameter);
    }
  }

  return std::nullopt;
}

knocknock::tApplicationParamsArray CApplicationParamData::GetApplicationParams( const int applicationId )
{
  knocknock::tApplicationParamsArray appParams = {};

  std::string getParamQuery = "SELECT APPLICATION_ID, NAME, PUBLIC, VALUE FROM APPLICATION_PARAMS WHERE APPLICATION_ID="+std::to_string(applicationId)+";";
  knocknock::CApplicationParam applicationParameter = {} ;

  auto getParamCallback = [](void *data, int argc, char **argv, char **azColName) {
    if (argc == 4 )
    {
      knocknock::tApplicationParamsArray* pAppParams = (knocknock::tApplicationParamsArray*)data;
      pAppParams->push_back(knocknock::CApplicationParam(atoi(argv[0]), argv[1], static_cast<bool>(atoi(argv[2])), argv[3]));
    }
    return 0;
  };

  m_rDBDriver.ExecuteSQLCommand( getParamQuery, getParamCallback, &appParams );
  return appParams;
}

knocknock::tApplicationParamsArray CApplicationParamData::GetAllParams()
{
  knocknock::tApplicationParamsArray appParams = {};

  std::string getParamQuery = "SELECT APPLICATION_ID, NAME, PUBLIC, VALUE FROM APPLICATION_PARAMS;";
  knocknock::CApplicationParam applicationParameter = {} ;

  auto getParamCallback = [](void *data, int argc, char **argv, char **azColName) {
    if (argc == 4 )
    {
      knocknock::tApplicationParamsArray* pAppParams = (knocknock::tApplicationParamsArray*)data;
      pAppParams->push_back(knocknock::CApplicationParam(atoi(argv[0]), argv[1], static_cast<bool>(atoi(argv[2])), argv[3]));
    }
    return 0;
  };

  m_rDBDriver.ExecuteSQLCommand( getParamQuery, getParamCallback, &appParams );

  return appParams;
}

bool CApplicationParamData::DeleteApplicationParams( int applicationId)
{
  std::string deleteParamQuery = "DELETE FROM APPLICATION_PARAMS WHERE APPLICATION_ID="+std::to_string(applicationId)+";";

  auto deleteCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };
  return m_rDBDriver.ExecuteSQLCommand( deleteParamQuery, deleteCallback, 0 );
}

bool CApplicationParamData::DeleteApplicationParameter( int applicationId, const std::string& paramName)
{
  std::string deleteParamQuery = "DELETE FROM APPLICATION_PARAMS WHERE APPLICATION_ID="+std::to_string(applicationId)+" AND NAME='"+paramName+"';";

  auto deleteCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };
  return m_rDBDriver.ExecuteSQLCommand( deleteParamQuery, deleteCallback, 0 );
}

}