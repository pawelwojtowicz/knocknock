#pragma once
#include <vector>
#include <memory>
#include "CSession.h"

namespace knocknock
{
class CConfiguration;

class ISessionPublisher
{
public:
  ISessionPublisher() = default;
  virtual ~ISessionPublisher() = default;

  virtual bool Initialize( const CConfiguration& rConfig ) = 0;
  virtual void Shutdown() = 0;
  
  virtual void PublishSession( const tSessionList& activeSessions ) = 0;
};

}