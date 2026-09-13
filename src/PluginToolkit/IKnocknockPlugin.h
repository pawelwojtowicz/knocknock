#pragma once
#include <string>
#include "ProducerMethods.h"

namespace knocknock
{
  
class IKnocknockPlugin
{
public:
  IKnocknockPlugin() = default;
  virtual ~IKnocknockPlugin() = default;

  virtual const std::string& GetName() const = 0;
  virtual const std::string& GetVersion() const = 0;

  virtual const tSessionPublisherBuildersMap& GetSessionPublisherBuilders() const = 0;
  virtual const tAuthenticationMethodBuildersMap& GetAuthenticationMethodBuilders() const = 0;
};

}

