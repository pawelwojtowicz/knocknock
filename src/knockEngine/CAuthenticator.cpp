#include "CAuthenticator.h"
#include "CAuthMethodFactory.h"
#include "CConfiguration.h"
#include "CSession.h"
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
  std::stringstream authMethodsStream(config.GetAllowedAuthMethods());
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

  return true;
}

void CAuthenticator::Shutdown()
{
  m_authMethods.clear();
}


std::string CAuthenticator::Login(CSession& session, const std::string& username, const std::string& password)
{
  const std::string& authMethod = session.GetAuthMethod();

  tAuthMethodMapCIterator cIter = m_authMethods.find(authMethod);
  if (cIter != m_authMethods.end())
  {
    return cIter->second->Login(session, username, password);
  } 

  session.UpdateUserSessionState(UserSessionState::AUTH_FAILED);
  return "Authentication failed";
}

std::string CAuthenticator::Authenticate(CSession& session, const std::string& authenticationPayload)
{
  const std::string& authMethod = session.GetAuthMethod();

  tAuthMethodMapCIterator cIter = m_authMethods.find(authMethod);
  if (cIter != m_authMethods.end())
  {
    return cIter->second->Authenticate(session, authenticationPayload);
  }

  session.UpdateUserSessionState(UserSessionState::AUTH_FAILED);
  return "Authentication failed";
}
}