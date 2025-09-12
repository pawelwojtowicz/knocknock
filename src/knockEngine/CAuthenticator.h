#pragma once
#include <string>
#include <map>
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

  std::string Login(CSession& session, const std::string& username, const std::string& password);

  std::string Authenticate(CSession& session, const std::string& authenticationPaylod);

private:
  tAuthMethodMap m_authMethods;
};
}