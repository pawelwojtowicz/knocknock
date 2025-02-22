#pragma once
#include <CApplicationParam.h>

namespace DBAccess
{

class IDBDriver;

class CApplicationParamData
{
public:
  CApplicationParamData( IDBDriver& dbDriver);
  ~CApplicationParamData() = default;

  bool AddApplicationParam( const knocknock::CApplicationParam& applicationParam );

private:
  IDBDriver& m_rDBDriver;
};
}