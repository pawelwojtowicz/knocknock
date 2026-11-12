#include "CSecurityIndustrialComplex.h"
//#include <Logger.h>
#include "CSimpleDBAuthMethod.h"
#include "CSHA256AuthMethod.h"
#include "CArgon2idAuthMethod.h"
#include "CSCRAuthMethod.h"
#include "CSQLiteSessionPublisher.h"

namespace knocknock
{
CSecurityIndustrialComplex::CSecurityIndustrialComplex()
{
  m_authMethodFactory.RegisterBuilder("simpledb", []() { return std::make_unique<CSimpleDBAuthMethod>(); });
  m_authMethodFactory.RegisterBuilder("sha256", []() { return std::make_unique<CSHA256AuthMethod>(); });
  m_authMethodFactory.RegisterBuilder("argon2id", []() { return std::make_unique<CArgon2idAuthMethod>(); });
  m_authMethodFactory.RegisterBuilder("scr", []() { return std::make_unique<CSCRAuthMethod>(); }); 
  
  m_sessionPublisherFactory.RegisterBuilder("express", []() { return std::make_unique<CSQLiteSessionPublisher>(); });
}

void CSecurityIndustrialComplex::ImportPlugins( const IPluginProvider& pluginProvider)
{
  for( const auto& [handle, plugin] : pluginProvider.GetPlugins() )
  {
    for( const auto& [name, builder] : plugin->GetSessionPublisherBuilders() )
    {
    //			LOG(INFO, "Loading publisher %s from %s", name.c_str(), plugin->GetName().c_str());
    m_sessionPublisherFactory.RegisterBuilder( name, builder );
    }
  }
}

}