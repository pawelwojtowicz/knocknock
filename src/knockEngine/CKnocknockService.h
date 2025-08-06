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
  virtual const CSession& Identify( const std::string& userId) override;
  virtual const CSession& Authenticate( const std::string& userId, const std::string& authenticationString) override;
private:
  std::map<std::string, CSession> m_sessions;

  static CSession empty;
};

}