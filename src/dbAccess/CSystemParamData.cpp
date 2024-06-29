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

  const std::string sqlQuery = "INSERT INTO SYSTEM_PARAMS (VALUE_KEY, VALUE) VALUES ('"+key+"','"+value+"');";

  auto insertValueCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, insertValueCallback, 0 ) ;
}

std::optional<std::string> CSystemParamData::GetSystemParam( const std::string& key)
{
  std::string paramValue = {};

  const std::string sqlQuery = "SELECT VALUE FROM SYSTEM_PARAMS WHERE VALUE_KEY = '" + key + "';";

  auto readingCallback = [](void *data, int argc, char **argv, char **azColName)
  {
    std::string* paramValuePtr = (std::string*)data;

    if ( 1 == argc )
    {
      *paramValuePtr = std::string(argv[0]);
    }

    return 0;
  };

  m_rDBDriver.ExecuteSQLCommand( sqlQuery, readingCallback, &paramValue );


  return !paramValue.empty() ? std::optional<std::string>(paramValue) : std::nullopt;
}

tSystemParameters CSystemParamData::GetAllSystemParams()
{
  tSystemParameters systemParameters = {};

  const std::string sqlQuery = "SELECT * FROM SYSTEM_PARAMS;";

  auto readingCallback = [](void *data, int argc, char **argv, char **azColName)
  {
    tSystemParameters* systemParamsMapPtr = (tSystemParameters*)data;

    if ( 2 == argc )
    {
      systemParamsMapPtr->insert(tSystemParameters::value_type(argv[0],argv[1]));
    }
    return 0;
  };

  m_rDBDriver.ExecuteSQLCommand( sqlQuery, readingCallback, &systemParameters );

  return systemParameters;
}




}