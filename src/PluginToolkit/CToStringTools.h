#pragma once
#include "knocknockTypes.h"

namespace knocknock
{
class CToStringTools
{
private:
  CToStringTools() = delete;
  ~CToStringTools() = delete;
public:
  static const char* SessionState( UserSessionState state );
};
}