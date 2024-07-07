#pragma once
#include <stdint.h>
#define MAX_LOG_TEXT_LENGTH 512

namespace Logger
{
class ILogger;

class CLoggerMsg
{
public:
  CLoggerMsg();
  CLoggerMsg( const uint16_t debugZone, const char* srcFileName, const uint32_t srcLineNo );
  virtual ~CLoggerMsg();

  void LogText( const char* logText, ... );

  static inline bool ZoneEnabled(uint16_t queryZone) { return ( 0 != (	sDebugZoneMask & queryZone ) );};

  static void SetLogger( ILogger* pLogger );

  static void SetDebugZone( const uint16_t dzMask);

  const char* GetDBGZone() const;
  const char* GetLogText() const;
  const char* GetSrcFileName() const;
  const uint32_t GetLineNo() const;
  const uint32_t GetTimeStamp() const;

private:
  uint16_t m_logDbgZone;
  char* m_logText;
  const char* m_pSrcFileName;
  uint32_t m_srcLineNo;
  uint32_t m_timeStamp;

private:
  static ILogger* spLogger;
  static uint16_t sDebugZoneMask;
};
}