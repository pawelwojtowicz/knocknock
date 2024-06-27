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

  auto insertValueCallback = [](std::vector<std::string> row) {
    return 0;
  };

  int rc = m_rDBDriver.ExecuteSQLCommand( sqlQuery, insertValueCallback );


  return 0 == rc ;



}

std::optional<std::string> CSystemParamData::GetSystemParam( const std::string& key)
{
  std::string outputString;
  const std::string sqlQuery = "SELECT VALUE FROM SYSTEM_PARAMS WHERE VALUE_KEY = '" + key + "';";

  auto getValueCallback = [&outputString](std::vector<std::string> row) {
    if ( 1 == row.size() )
    {
      outputString = row[0];
    }
    return 0;
  };

  m_rDBDriver.ExecuteSQLCommand( sqlQuery, getValueCallback );


  return outputString;
}



}