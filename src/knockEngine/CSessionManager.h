#pragma once
#include "IKnocknockService.h"
#include <cstdint>
#include <map>
#include <shared_mutex>
#include "CSession.h"
#include "CSessionBuilder.h"
#include "CAuthenticator.h"
#include "CLoginRateLimiter.h"
#include "CPolicyGuard.h"
#include "CSessionPublisher.h"

namespace DBAccess
{
  class IDBAccess;
}

namespace knocknock
{
class CConfiguration;


class CSessionManager : public IKnocknockService
{
public:
  CSessionManager( DBAccess::IDBAccess& rDBAccess, CConfiguration& rConfiguration );
  virtual ~CSessionManager();

  bool Initialize();
  void Tick();
  void Shutdown();
private:
  const CSession Login(const tKeyValueMap& input, tKeyValueMap& output) override;
  const CSession Authenticate(const tKeyValueMap& input, tKeyValueMap& output) override;
  const CSession Touch(const tKeyValueMap& input, tKeyValueMap& output) override;
  bool Logout(const tKeyValueMap& input, tKeyValueMap& output) override;

private:
  //internal utility method to cleanup expired sessions, called periodically by Tick()
  void CleanupExpiredSessions();

  //internal utility method to hand a snapshot of active sessions to the session publisher, called periodically by Tick()
  void PublishActiveSessions();

private:
  DBAccess::IDBAccess& m_rDBAccess;

  CConfiguration& m_rConfiguration;

  CSessionBuilder m_sessionBuilder;

  CAuthenticator m_authenticator;

  const CSession m_emptySession {"", "", "", "", ""}; // Represents an empty session, used when no valid session is found

  std::shared_mutex m_sessionsMutex;

  tSessionMap m_sessions;

  int64_t m_sessionExpirationTimeout;

  int64_t m_authenticationChallengeTimeout;

  CLoginRateLimiter m_loginRateLimiter;

  CPolicyGuard m_policyGuard;

  CSessionPublisher m_sessionPublisher;
};

}