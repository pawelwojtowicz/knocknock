#include "CPluginBase.h"

namespace knocknock
{
CPluginBase::CPluginBase( const std::string& name, const std::string& version )
: m_name( name )
, m_version( version )
{
}

const std::string& CPluginBase::GetName() const
{
  return m_name;
}

const std::string& CPluginBase::GetVersion() const
{
  return m_version;
}

const tSessionPublisherBuildersMap& CPluginBase::GetSessionPublisherBuilders() const
{
  return m_sessionPublisherBuilders;
}

const tAuthenticationMethodBuildersMap& CPluginBase::GetAuthenticationMethodBuilders() const
{
  return m_authenticationMethodBuilders;
}

}