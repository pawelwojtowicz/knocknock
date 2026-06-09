#pragma once
#include <vector>
#include <memory>
#include "CSession.h"

namespace knocknock
{

class ISessionPublisher
{
public:
  ISessionPublisher() = default;
  virtual ~ISessionPublisher() = default;

  virtual bool Initialize() = 0;
  virtual void Shutdown() = 0;
  
  virtual void PublishSession( const tSessionList& activeSessions ) = 0;
};

}