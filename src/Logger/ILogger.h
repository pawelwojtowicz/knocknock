#pragma once

namespace Logger
{
class CLoggerMsg;

class ILogger
{
public:
  ILogger() = default;
  virtual ~ILogger() = default;

  virtual void RecordDebugMessage( const CLoggerMsg& rLoggerMsg) = 0;
};
}