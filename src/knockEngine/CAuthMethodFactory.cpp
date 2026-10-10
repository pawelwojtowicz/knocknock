#include "CAuthMethodFactory.h"
#include "CSimpleDBAuthMethod.h"
#include "CSHA256AuthMethod.h"
#include "CArgon2idAuthMethod.h"
#include "CSCRAuthMethod.h"

namespace knocknock
{
CAuthMethodFactory::CAuthMethodFactory()
{
  RegisterBuilder("simpledb", []() { return std::make_unique<CSimpleDBAuthMethod>(); });
  RegisterBuilder("sha256", []() { return std::make_unique<CSHA256AuthMethod>(); });
  RegisterBuilder("argon2id", []() { return std::make_unique<CArgon2idAuthMethod>(); });
  RegisterBuilder("scr", []() { return std::make_unique<CSCRAuthMethod>(); });
}

} // namespace knocknock