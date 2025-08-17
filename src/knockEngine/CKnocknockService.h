#pragma once
#include "IKnocknockService.h"
#include <map>
#include "CSession.h"

namespace knocknock
{

class CKnocknockService : public IKnocknockService
{
public:
  CKnocknockService();
  virtual ~CKnocknockService();

  bool Initialize();
  void Shutdown();
private:
  const std::string Login(const std::string& userId, const std::string& password) override;
  const std::string Authenticate(const std::string& sessionId, const std::string& authenticationPayload) override;
  const std::string Logout(const std::string& sessionId) override;
  const std::string Touch(const std::string& sessionId) override;
private:
  std::map<std::string, CSession> m_sessions;

  static CSession empty;
};

}