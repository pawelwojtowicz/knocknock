#include "CAuthenticator.h"
#include "CAuthMethodFactory.h"
#include "CConfiguration.h"
#include "CSession.h"
#include "CKeyValueHelper.h"
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
  std::stringstream authMethodsStream(config.GetParamString(cParamName_AllowedAuthMethods));
  std::string method;

  while (std::getline(authMethodsStream, method, ','))
  {
    auto authMethod = authMethodFactory.CreateAuthMethod(method);
    if (authMethod)
    {
      m_authMethods[method] = std::move(authMethod);
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