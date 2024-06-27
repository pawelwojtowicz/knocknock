#pragma once
#include <string>
#include <optional>

namespace DBAccess
{
class IDBDriver;

class CSystemParamData
{
public:
  CSystemParamData(IDBDriver& rDBDriver);
  virtual ~CSystemParamData();

  bool AddSystemParam( const std::string& key, const std::string& value);

  std::optional<std::string> GetSystemParam( const std::string& key);

private:
  IDBDriver& m_rDBDriver;
};
}