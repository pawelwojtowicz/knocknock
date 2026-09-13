#include "CAuthenticator.h"
#include "CAuthMethodFactory.h"
#include "CConfiguration.h"
#include "CSession.h"
#include "CKeyValueHelper.h"
#include <CTokenizer.h>
#include <sstream>

namespace knocknock
{

CAuthenticator::CAuthenticator()
{
}

CAuthenticator::~CAuthenticator()
{
}

bool CAuthenticator::Initialize( const CConfiguration& config )
{
  CAuthMethodFactory authMethodFactory;
  const auto authMethodTokens = CTokenizer::Tokenize(config.GetParamString(cParamName_AllowedAuthMethods, cParamValue_AllowedAuthMethods), ',');

  for (const auto& token : authMethodTokens)
  {
    auto authMethod = authMethodFactory.CreateAuthMethod(token);
    if (authMethod)
    {
      m_authMethods.insert_or_assign(token, std::move(authMethod));
    }
    else
    {
      return false;
    }
  }

  return ( m_authMethods.size() > 0 );
}

void CAuthenticator::Shutdown()
{
  m_authMethods.clear();
}


tKeyValueMap CAuthenticator::Login(CSession& session, const tKeyValueMap& loginPayload)
{
  const std::string& authMethod = session.GetAuthMethod();

  tAuthMethodMapCIterator cIter = m_authMethods.find(authMethod);
  if (cIter != m_authMethods.end())
  {
    return cIter->second->Login(session, loginPayload);
  } 

  session.UpdateUserSessionState(UserSessionState::AUTH_FAILED);
  return {tKeyValueMap::value_type("error", "Authentication failed")};
}

tKeyValueMap CAuthenticator::Authenticate(CSession& session, const tKeyValueMap& authenticationPayload)
{
  const std::string& authMethod = session.GetAuthMethod();

  tAuthMethodMapCIterator cIter = m_authMethods.find(authMethod);
  if (cIter != m_authMethods.end())
  {
    return cIter->second->Authenticate(session, authenticationPayload);
  }

  session.UpdateUserSessionState(UserSessionState::AUTH_FAILED);
  return tKeyValueMap{ { "error", "Authentication failed" } };
}
}