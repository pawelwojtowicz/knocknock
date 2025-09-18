#include "CConfiguration.h"

namespace knocknock
{

constexpr auto cParamName_AnonymousUserTemplate = "anonymousUserTemplate";
constexpr auto cParamName_DefaultAuthenticationMethod = "defaultAuthenticationMethod";
constexpr auto cParamName_DefaultAuthenticationString = "defaultAuthenticationString";
constexpr auto cParamName_AllowedAuthMethods = "allowedAuthMethods";

bool CConfiguration::LoadConfig(DBAccess::IDBAccess& rDBAccess)
{
  std::optional<std::string> paramValue;

  paramValue = rDBAccess.GetSystemParamData().GetSystemParam(cParamName_AnonymousUserTemplate);
  if (paramValue.has_value())
  {
    m_anonumousUserTemplate = paramValue.value();
  }

  paramValue = rDBAccess.GetSystemParamData().GetSystemParam(cParamName_DefaultAuthenticationMethod);
  if (paramValue.has_value())
  {
    m_defaultAuthenticationMethod = paramValue.value();
  }

  paramValue = rDBAccess.GetSystemParamData().GetSystemParam(cParamName_DefaultAuthenticationString);
  if (paramValue.has_value())
  {
    m_defaultAuthenticationString = paramValue.value();
  }

  paramValue = rDBAccess.GetSystemParamData().GetSystemParam(cParamName_AllowedAuthMethods);
  if (paramValue.has_value())
  {
    m_allowedAuthMethods = paramValue.value();
  }

  return true;
}

}