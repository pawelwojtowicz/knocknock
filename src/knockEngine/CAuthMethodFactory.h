#pragma once
#include <string>
#include <memory>
#include "IAuthenticationMethod.h"

namespace knocknock
{
class CAuthMethodFactory
{
public:
  CAuthMethodFactory() = default;
  virtual ~CAuthMethodFactory() = default;
  
  std::unique_ptr<IAuthenticationMethod> CreateAuthMethod(const std::string& type);
};
}