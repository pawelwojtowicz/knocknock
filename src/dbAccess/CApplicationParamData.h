#pragma once
#include <CApplicationParam.h>
#include <optional>

namespace DBAccess
{

class IDBDriver;

class CApplicationParamData
{
public:
  CApplicationParamData( IDBDriver& dbDriver);
  ~CApplicationParamData() = default;

  bool AddApplicationParam( const knocknock::CApplicationParam& applicationParam );

  bool UpdateApplicationParam( const knocknock::CApplicationParam& applicationParam );

  std::optional<knocknock::CApplicationParam> GetApplicationParam( const int applicationId, const std::string& paramName);

  knocknock::tApplicationParamsArray GetApplicationParams( const int applicationI );

  knocknock::tApplicationParamsArray GetAllParams();

  bool DeleteApplicationParams( int applicationId);

  bool DeleteApplicationParameter( int applicationId, const std::string& paramName);

private:
  IDBDriver& m_rDBDriver;
};
}