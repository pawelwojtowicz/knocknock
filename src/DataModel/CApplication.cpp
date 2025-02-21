#include "CApplication.h"

namespace knocknock
{
CApplication::CApplication()
: m_appId{-1}
, m_name{}
, m_dataPublisher{}
, m_accessToken{}
{
}

CApplication::CApplication( const int id, const std::string& name, const std::string& dataPublisher, const std::string& accessToken)
: m_appId(id)
, m_name(name)
, m_dataPublisher(dataPublisher)
, m_accessToken(accessToken)
{
}

}