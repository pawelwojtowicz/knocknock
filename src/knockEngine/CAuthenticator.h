#pragma once
#include <memory>
#include "IAuthenticationMethod.h"


namespace knocknock
{
class CSession;

class CConfiguration;

class CAuthenticator
{
public:
  using tAuthMethodMap = std::map<std::string, std::unique_ptr<IAuthenticationMethod>>;
  using tAuthMethodMapCIterator = tAuthMethodMap::const_iterator;
  CAuthenticator();
  ~CAuthenticator();

  bool Initialize( const CConfiguration& config ); 
  void Shutdown();

  tKeyValueMap Login(CSession& session, const std::string& password);

  tKeyValueMap Authenticate(CSession& session, const tKeyValueMap& authenticationPayload);

private:
  tAuthMethodMap m_authMethods;
};
}