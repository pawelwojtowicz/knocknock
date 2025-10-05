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

  virtual const CSession& Login(const tKeyValueMap& input, tKeyValueMap& output) = 0;
  virtual const CSession& Authenticate(const tKeyValueMap& input, tKeyValueMap& output) = 0;
  virtual const bool Logout(const tKeyValueMap& input, tKeyValueMap& output) = 0;
  virtual const bool Touch(const tKeyValueMap& input, tKeyValueMap& output) = 0;

private:
  IKnocknockService(const IKnocknockService&) = delete;
  IKnocknockService& operator=(const IKnocknockService&) = delete; 
};

}