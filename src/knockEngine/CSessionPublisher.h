#pragma once
#include "CSession.h"

namespace knocknock
{
class CConfiguration;

class CSessionPublisher
{
public:
  CSessionPublisher() = default;
  virtual ~CSessionPublisher() = default;

  bool Initialize( CConfiguration& config );
  void Shutdown();

}; 
}