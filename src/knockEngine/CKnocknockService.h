#pragma once
#include "IKnocknockService.h"
#include <map>
#include "CSession.h"
#include "CSessionBuilder.h"
#include "CAuthenticator.h"

namespace DBAccess
{
  class IDBAccess;
}

namespace knocknock
{
class CConfiguration;


class CKnocknockService : public IKnocknockService
{
  using tSessionMap = std::map<std::string, CSession>;
public:
  CKnocknockService( DBAccess::IDBAccess& rDBAccess, CConfiguration& rConfiguration );
  virtual ~CKnocknockService();

  bool Initialize();
  void Shutdown();
private:
  const tKeyValueMap Login(const std::string& userId, const std::string& password) override;
  const tKeyValueMap Authenticate(const std::string& sessionId, const tKeyValueMap& authenticationPayload) override;
  const tKeyValueMap Logout(const std::string& sessionId) override;
  const tKeyValueMap Touch(const std::string& sessionId) override;
private:
  DBAccess::IDBAccess& m_rDBAccess;

  CConfiguration& m_rConfiguration;

  CSessionBuilder m_sessionBuilder;

  CAuthenticator m_authenticator;

  tSessionMap m_sessions;

  static CSession empty;
};

}