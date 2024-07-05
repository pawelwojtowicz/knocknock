#pragma once
#include <string>

namespace knocknock
{
class CUserSession;

class IKnocknockService
{
public:
  IKnocknockService() = default;
  virtual ~IKnocknockService() = default;

  virtual const CUserSession& Identify( const std::string& userId) = 0;
  virtual const CUserSession& Authenticate( const std::string& userId, const std::string& authenticationString) = 0;

private:
  IKnocknockService(const IKnocknockService&) = delete;
  IKnocknockService& operator=(const IKnocknockService&) = delete; 
};

}