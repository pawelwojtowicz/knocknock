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

  virtual const tKeyValueMap Login( const std::string& userId, const std::string& password ) = 0;
  virtual const tKeyValueMap Authenticate( const std::string& sessionId, const tKeyValueMap& authenticationPayload ) = 0;
  virtual const tKeyValueMap Logout( const std::string& sessionId ) = 0;
  virtual const tKeyValueMap Touch( const std::string& sessionId ) = 0;

private:
  IKnocknockService(const IKnocknockService&) = delete;
  IKnocknockService& operator=(const IKnocknockService&) = delete; 
};

}