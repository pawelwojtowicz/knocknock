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
  const tKeyValueMap Login(const std::string& userId, const std::string& password) override;
  const tKeyValueMap Authenticate(const std::string& sessionId, const tKeyValueMap& authenticationPayload) override;
  const tKeyValueMap Logout(const std::string& sessionId) override;
  const tKeyValueMap Touch(const std::string& sessionId) override;
private:
  std::map<std::string, CSession> m_sessions;

  static CSession empty;
};

}