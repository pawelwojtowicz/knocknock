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

  std::cout << sqlQuery << std::endl;

  return m_rDBDriver.ExecuteSQLCommand( sqlQuery, insertValueCallback, 0 ) ;
}


}