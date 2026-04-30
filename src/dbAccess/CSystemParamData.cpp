#include "CSystemParamData.h"
#include "IDBDriver.h"

namespace DBAccess
{

CSystemParamData::CSystemParamData( IDBDriver& rDBDriver)
: m_rDBDriver( rDBDriver )
{

}

bool CSystemParamData::AddSystemParam( const std::string& key, const std::string& value)
{
  const std::string sql = "INSERT INTO SYSTEM_PARAMS (VALUE_KEY, VALUE) VALUES (?, ?);";
  std::vector<std::string> params = { key, value };

  return m_rDBDriver.ExecutePreparedStatement(sql, params, nullptr, nullptr);
}

std::optional<std::string> CSystemParamData::GetSystemParam( const std::string& key)
{
  std::string paramValue = {};

  const std::string sql = "SELECT VALUE FROM SYSTEM_PARAMS WHERE VALUE_KEY = ?;";
  std::vector<std::string> params = { key };

  auto readingCallback = [](void *data, int argc, char **argv, char **azColName)
  {
    std::string* paramValuePtr = (std::string*)data;

    if ( 1 == argc )
    {
      *paramValuePtr = std::string(argv[0]);
    }

    return 0;
  };

  m_rDBDriver.ExecutePreparedStatement(sql, params, readingCallback, &paramValue);

  return !paramValue.empty() ? std::optional<std::string>(paramValue) : std::nullopt;
}

tSystemParameters CSystemParamData::GetAllSystemParams()
{
  tSystemParameters systemParameters = {};

  const std::string sql = "SELECT * FROM SYSTEM_PARAMS;";

  auto readingCallback = [](void *data, int argc, char **argv, char **azColName)
  {
    tSystemParameters* systemParamsMapPtr = (tSystemParameters*)data;

    if ( 2 == argc )
    {
      systemParamsMapPtr->insert(tSystemParameters::value_type(argv[0],argv[1]));
    }
    return 0;
  };

  m_rDBDriver.ExecutePreparedStatement(sql, {}, readingCallback, &systemParameters);

  return systemParameters;
}




}