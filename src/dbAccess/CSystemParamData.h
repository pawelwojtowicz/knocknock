#pragma once
#include "ISystemParamData.h"
namespace DBAccess
{

class IDBDriver;

class CSystemParamData : public ISystemParamData
{
public:
  CSystemParamData(IDBDriver& rDBDriver);
  virtual ~CSystemParamData() = default;

  bool AddSystemParam( const std::string& key, const std::string& value) override;

  tSystemParameters GetAllSystemParams()  override;

  std::optional<std::string> GetSystemParam( const std::string& key) override;

private:
  IDBDriver& m_rDBDriver;
};
}