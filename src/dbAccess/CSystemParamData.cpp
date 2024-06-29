#include "CSystemParamData.h"
#include "IDBDriver.h"

namespace DBAccess
{

CSystemParamData::CSystemParamData( IDBDriver& rDBDriver)
: m_rDBDriver( rDBDriver )
{

}

CSystemParamData::~CSystemParamData()
{

}

bool CSystemParamData::AddSystemParam( const std::string& key, const std::string& value)
{

  const std::string sqlQuery = "INSERT INTO SYSTEM_PARAMS (VALUE_KEY, VALUE) VALUES ('"+key+"','"+value+"');";

  auto insertValueCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  int rc = m_rDBDriver.ExecuteSQLCommand( sqlQuery, insertValueCallback, 0 );


  return 0 == rc ;



}

std::optional<std::string> CSystemParamData::GetSystemParam( const std::string& key)
{
  using tSystemParameter = std::pair<std::string, std::string>;
  tSystemParameter retVal;

  const std::string sqlQuery = "SELECT * FROM SYSTEM_PARAMS WHERE VALUE_KEY = '" + key + "';";

  auto readingCallback = [](void *data, int argc, char **argv, char **azColName)
  {
    tSystemParameter* retValPtr = (tSystemParameter*)data;

    if ( 2 == argc )
    {
      *retValPtr = std::make_pair(argv[0],argv[1]);
    }

    return 0;
  };

  m_rDBDriver.ExecuteSQLCommand( sqlQuery, readingCallback, &retVal );


  return retVal.second;
}



}