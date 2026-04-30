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
  const std::string sql = "INSERT INTO APPLICATION_PARAMS (APPLICATION_ID, NAME, PUBLIC, VALUE) VALUES (?, ?, ?, ?);";
  std::vector<std::string> params = {
    std::to_string(applicationParam.GetApplicationId()),
    applicationParam.GetParameterName(),
    std::to_string(applicationParam.IsPublic() ? 1 : 0),
    applicationParam.GetValue()
  };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

bool CApplicationParamData::UpdateApplicationParam( const knocknock::CApplicationParam& applicationParam )
{
  const std::string sql = "UPDATE APPLICATION_PARAMS SET PUBLIC=?, VALUE=? WHERE APPLICATION_ID=? AND NAME=?;";
  std::vector<std::string> params = {
    std::to_string(applicationParam.IsPublic() ? 1 : 0),
    applicationParam.GetValue(),
    std::to_string(applicationParam.GetApplicationId()),
    applicationParam.GetParameterName()
  };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

std::optional<knocknock::CApplicationParam> CApplicationParamData::GetApplicationParam( const int applicationId, const std::string& paramName)
{
  const std::string sql = "SELECT APPLICATION_ID, NAME, PUBLIC, VALUE FROM APPLICATION_PARAMS WHERE APPLICATION_ID=? AND NAME=?;";
  std::vector<std::string> params = { std::to_string(applicationId), paramName };

  knocknock::CApplicationParam applicationParameter = {};

  auto getParamCallback = [](void *data, int argc, char **argv, char **azColName) {
    if (argc == 4)
    {
      knocknock::CApplicationParam* pAppParam( (knocknock::CApplicationParam*)data);
      *pAppParam = knocknock::CApplicationParam(atoi(argv[0]), argv[1], static_cast<bool>(atoi(argv[2])), argv[3]);
    }
    return 0;
  };

  m_rDBDriver.ExecutePreparedStatement(sql, params, getParamCallback, &applicationParameter);

  if ( applicationParameter.GetApplicationId() == applicationId )
  {
    return std::optional<knocknock::CApplicationParam>(applicationParameter);
  }

  return std::nullopt;
}

knocknock::tApplicationParamsArray CApplicationParamData::GetApplicationParams( const int applicationId )
{
  knocknock::tApplicationParamsArray appParams = {};

  const std::string sql = "SELECT APPLICATION_ID, NAME, PUBLIC, VALUE FROM APPLICATION_PARAMS WHERE APPLICATION_ID=?;";
  std::vector<std::string> params = { std::to_string(applicationId) };

  auto getParamCallback = [](void *data, int argc, char **argv, char **azColName) {
    if (argc == 4)
    {
      knocknock::tApplicationParamsArray* pAppParams = (knocknock::tApplicationParamsArray*)data;
      pAppParams->push_back(knocknock::CApplicationParam(atoi(argv[0]), argv[1], static_cast<bool>(atoi(argv[2])), argv[3]));
    }
    return 0;
  };

  m_rDBDriver.ExecutePreparedStatement(sql, params, getParamCallback, &appParams);
  return appParams;
}

knocknock::tApplicationParamsArray CApplicationParamData::GetAllParams()
{
  knocknock::tApplicationParamsArray appParams = {};

  const std::string sql = "SELECT APPLICATION_ID, NAME, PUBLIC, VALUE FROM APPLICATION_PARAMS;";

  auto getParamCallback = [](void *data, int argc, char **argv, char **azColName) {
    if (argc == 4)
    {
      knocknock::tApplicationParamsArray* pAppParams = (knocknock::tApplicationParamsArray*)data;
      pAppParams->push_back(knocknock::CApplicationParam(atoi(argv[0]), argv[1], static_cast<bool>(atoi(argv[2])), argv[3]));
    }
    return 0;
  };

  m_rDBDriver.ExecutePreparedStatement(sql, {}, getParamCallback, &appParams);

  return appParams;
}

bool CApplicationParamData::DeleteApplicationParams( int applicationId)
{
  const std::string sql = "DELETE FROM APPLICATION_PARAMS WHERE APPLICATION_ID=?;";
  std::vector<std::string> params = { std::to_string(applicationId) };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

bool CApplicationParamData::DeleteApplicationParameter( int applicationId, const std::string& paramName)
{
  const std::string sql = "DELETE FROM APPLICATION_PARAMS WHERE APPLICATION_ID=? AND NAME=?;";
  std::vector<std::string> params = { std::to_string(applicationId), paramName };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

}