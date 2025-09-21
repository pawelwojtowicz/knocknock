#pragma once
#include <string>
#include <IDBAccess.h>
#include "knocknockTypes.h"

namespace knocknock
{
constexpr auto cParamName_AnonymousUserTemplate = "anonymousUserTemplate";
constexpr auto cParamName_DefaultAuthenticationMethod = "defaultAuthenticationMethod";
constexpr auto cParamName_DefaultAuthenticationString = "defaultAuthenticationString";
constexpr auto cParamName_AllowedAuthMethods = "allowedAuthMethods";

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

  const std::string GetParamString( const std::string& paramName ) const;

private:
  tParameterMap m_parameters;

  int m_sessionMaxAge = 0;
  bool m_cookieHttpOnly = false;
  std::string m_cookiePath = "/";
  bool m_cookieSecure = false;
  CookieSameSite m_cookieSameSite = CookieSameSite::LAX;

};
}