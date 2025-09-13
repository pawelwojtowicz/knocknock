#include "CAuthMethodFactory.h"
#include "CSimpleDBAuthMethod.h"
#include "CSHA256AuthMethod.h"
#include "CSCRAuthMethod.h"

namespace knocknock
{
std::unique_ptr<IAuthenticationMethod> CAuthMethodFactory::CreateAuthMethod(const std::string& type)
{
  if (type == "simpledb")
  {
    return std::make_unique<CSimpleDBAuthMethod>();
  }
  else if (type == "sha256")
  {
    return std::make_unique<CSHA256AuthMethod>();
  }
  else if (type == "scr")
  {
    return std::make_unique<CSCRAuthMethod>();
  }
  return nullptr;
}
} // namespace knocknock