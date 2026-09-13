#pragma once
#include <string>
#include <memory>
#include <ProducerMethods.h>
#include "IAuthenticationMethod.h"

namespace knocknock
{
class CAuthMethodFactory
{
public:
  CAuthMethodFactory();
  virtual ~CAuthMethodFactory() = default;
  
  std::unique_ptr<IAuthenticationMethod> CreateAuthMethod(const std::string& type);

private:
  tAuthenticationMethodBuildersMap m_authMethodBuilders;
};
}