#pragma once
#include <stdint.h>
#include "ILogger.h"
#include "Logger.h"

namespace Logger
{

class CSimpleLogger : public ILogger
{
public:
  using tLogEntryCfg = enum
  {
    eTimeStamp  = 0x01,
    eZone       = 0x02,
    eLogEntry   = 0x04,
    eSource     = 0x08,
    eComplete   = eTimeStamp | eZone | eLogEntry | eSource
  };
  CSimpleLogger( const uint16_t loggerConfig = ALL, tLogEntryCfg entryConfig = tLogEntryCfg::eComplete );
  virtual ~CSimpleLogger();
private:
  void RecordDebugMessage( const CLoggerMsg& rLoggerMsg) override ;
private:
  tLogEntryCfg m_logEntryConfig;
};
}