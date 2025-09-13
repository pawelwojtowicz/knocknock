#pragma once
#include <string>
#include "knocknockTypes.h"

namespace knocknock
{
class CSession;

class IKnocknockService
{
public:
  IKnocknockService() = default;
  virtual ~IKnocknockService() = default;

  virtual const std::string Login( const std::string& userId, const std::string& password ) = 0;
  virtual const std::string Authenticate( const std::string& sessionId, const std::string& authenticationPayload ) = 0;
  virtual const std::string Logout( const std::string& sessionId ) = 0;
  virtual const std::string Touch( const std::string& sessionId ) = 0;

private:
  IKnocknockService(const IKnocknockService&) = delete;
  IKnocknockService& operator=(const IKnocknockService&) = delete; 
};

}