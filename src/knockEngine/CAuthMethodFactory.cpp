#include "CAuthMethodFactory.h"
#include "CSimpleDBAuthMethod.h"
#include "CSHA256AuthMethod.h"
#include "CArgon2idAuthMethod.h"
#include "CSCRAuthMethod.h"

namespace knocknock
{
CAuthMethodFactory::CAuthMethodFactory()
{
  m_authMethodBuilders.insert({ "simpledb", []() { return std::make_unique<CSimpleDBAuthMethod>(); } });
  m_authMethodBuilders.insert({ "sha256", []() { return std::make_unique<CSHA256AuthMethod>(); } });
  m_authMethodBuilders.insert({ "argon2id", []() { return std::make_unique<CArgon2idAuthMethod>(); } });
  m_authMethodBuilders.insert({ "scr", []() { return std::make_unique<CSCRAuthMethod>(); } });
}
std::unique_ptr<IAuthenticationMethod> CAuthMethodFactory::CreateAuthMethod(const std::string& type)
{
  auto it = m_authMethodBuilders.find(type);
  if (it != m_authMethodBuilders.end())
  {
    return it->second();
  }
  return nullptr;
}

} // namespace knocknock