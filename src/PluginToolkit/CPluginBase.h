 #pragma once
#include "IKnocknockPlugin.h"

namespace knocknock
{
class CPluginBase : public IKnocknockPlugin
{
public:
  CPluginBase( const std::string& name, const std::string& version );
  virtual ~CPluginBase() = default;

  const std::string& GetName() const override;
  const std::string& GetVersion() const override;

  const tSessionPublisherBuildersMap& GetSessionPublisherBuilders() const override;
  const tAuthenticationMethodBuildersMap& GetAuthenticationMethodBuilders() const override;

protected:
  void RegisterSessionPublisher( const std::string& name, tSessionPublisherBuilder creationMethod);

private:
  std::string m_name;
  std::string m_version;

  tSessionPublisherBuildersMap m_sessionPublisherBuilders;
  tAuthenticationMethodBuildersMap m_authenticationMethodBuilders;
};
}