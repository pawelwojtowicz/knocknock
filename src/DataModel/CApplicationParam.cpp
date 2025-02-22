#include "CApplicationParam.h"

namespace knocknock
{
CApplicationParam::CApplicationParam( const int applicationId, const std::string& paramName, bool isPublic, const std::string& value)
: m_applicationId(applicationId)
, m_paramName(paramName)
, m_public(isPublic)
, m_value(value)
{
}

}