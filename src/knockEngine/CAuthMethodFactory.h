#pragma once
#include <TBuilderFactory.h>
#include "IAuthenticationMethod.h"

namespace knocknock
{
class CAuthMethodFactory : public TBuilderFactory<IAuthenticationMethod>
{
public:
  CAuthMethodFactory();
};
}