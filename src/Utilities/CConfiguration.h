#pragma once
#include <string>
#include <IDBAccess.h>

namespace knocknock
{

class CConfiguration
{
  using tParameterMap = std::map<std::string, std::string>;
  using tParameterMapCI = tParameterMap::const_iterator;
  using tParameterMapI = tParameterMap::iterator;
public:
  CConfiguration();
  ~CConfiguration() = default;


  bool LoadConfig(DBAccess::IDBAccess& rDBAccess);
  bool LoadConfig(const std::string& filename);

  const std::string GetParamString( const std::string& paramName, const std::string defaultValue = {}) const;

  int GetParamInt( const std::string& paramName, int defaultValue = 0 ) const;

private:
  tParameterMap m_parameters;
};
}