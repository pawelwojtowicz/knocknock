#pragma once
#include <string>

namespace knocknock
{
class CSession;

class IKnocknockService
{
public:
  IKnocknockService() = default;
  virtual ~IKnocknockService() = default;

  virtual const CSession& Identify( const std::string& userId) = 0;
  virtual const CSession& Authenticate( const std::string& userId, const std::string& authenticationString) = 0;

private:
  IKnocknockService(const IKnocknockService&) = delete;
  IKnocknockService& operator=(const IKnocknockService&) = delete; 
};

}