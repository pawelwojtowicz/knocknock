#pragma once
#include <string>
#include <map>
#include <optional>

namespace DBAccess
{

using tSystemParameters = std::map<std::string,std::string>; 
class IDBDriver;

class CSystemParamData
{
public:
  CSystemParamData(IDBDriver& rDBDriver);
  virtual ~CSystemParamData();

  bool AddSystemParam( const std::string& key, const std::string& value);

  tSystemParameters GetAllSystemParams();

  std::optional<std::string> GetSystemParam( const std::string& key);

private:
  IDBDriver& m_rDBDriver;
};
}