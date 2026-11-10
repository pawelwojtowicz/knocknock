#include "CSecurityIndustrialComplex.h"
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

}