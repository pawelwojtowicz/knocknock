#include "CUserSetting.h"

namespace knocknock
{


CUserSetting::CUserSetting( const int appId, const std::string& paramName, const std::string& userId, const std::string& value)
: m_applicationId(appId)
, m_paramName(paramName)
, m_userId(userId)
, m_value(value)
{
  
}

}