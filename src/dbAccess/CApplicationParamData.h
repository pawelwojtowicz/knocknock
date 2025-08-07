#pragma once
#include "IApplicationParamData.h"

namespace DBAccess
{

class IDBDriver;

class CApplicationParamData : public IApplicationParamData
{
public:
  CApplicationParamData( IDBDriver& dbDriver);
  ~CApplicationParamData() = default;

  bool AddApplicationParam( const knocknock::CApplicationParam& applicationParam ) override;

  bool UpdateApplicationParam( const knocknock::CApplicationParam& applicationParam ) override;

  std::optional<knocknock::CApplicationParam> GetApplicationParam( const int applicationId, const std::string& paramName) override;

  knocknock::tApplicationParamsArray GetApplicationParams( const int applicationI ) override;

  knocknock::tApplicationParamsArray GetAllParams() override;

  bool DeleteApplicationParams( int applicationId) override;

  bool DeleteApplicationParameter( int applicationId, const std::string& paramName) override;

private:
  IDBDriver& m_rDBDriver;
};
}