#pragma once
#include "IKnocknockService.h"
#include <map>
#include "CSession.h"
#include "CSessionBuilder.h"
#include "CAuthenticator.h"
#include "CSessionVerifier.h"

namespace DBAccess
{
  class IDBAccess;
}

namespace knocknock
{
class CConfiguration;


class CSessionManager : public IKnocknockService
{
  using tSessionMap = std::map<std::string, CSession>;
public:
  CSessionManager( DBAccess::IDBAccess& rDBAccess, CConfiguration& rConfiguration );
  virtual ~CSessionManager();

  bool Initialize();
  void Shutdown();
private:
  const CSession& Login(const tKeyValueMap& input, tKeyValueMap& output) override;
  const CSession& Authenticate(const tKeyValueMap& input, tKeyValueMap& output) override;
  const bool Logout(const tKeyValueMap& input, tKeyValueMap& output) override;
  const bool Touch(const tKeyValueMap& input, tKeyValueMap& output) override;
private:
  DBAccess::IDBAccess& m_rDBAccess;

  CConfiguration& m_rConfiguration;

  CSessionBuilder m_sessionBuilder;

  CAuthenticator m_authenticator;

  CSessionVerifier m_sessionVerifier;

  tSessionMap m_sessions;

  static CSession empty;
};

}