#pragma once
#include "IKnocknockService.h"
#include <map>
#include "CUserSession.h"

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
  virtual const CUserSession& Identify( const std::string& userId) override;
  virtual const CUserSession& Authenticate( const std::string& userId, const std::string& authenticationString) override;
private:
  std::map<std::string, CUserSession> m_sessions;

  static CUserSession empty;
};

}