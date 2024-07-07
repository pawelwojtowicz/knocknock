#include "CSimpleLogger.h"
#include "CLoggerMsg.h"
#include <iostream>


namespace Logger
{



CSimpleLogger::CSimpleLogger(const uint16_t loggerConfig, tLogEntryCfg entryConfig)
: m_logEntryConfig( entryConfig )
{
  CLoggerMsg::SetDebugZone(loggerConfig);
  CLoggerMsg::SetLogger(this);
}

CSimpleLogger::~CSimpleLogger()
{

}

void CSimpleLogger::RecordDebugMessage( const CLoggerMsg& rLoggerMsg)
{
  if ( tLogEntryCfg::eTimeStamp & m_logEntryConfig )
  {
    std::cout << rLoggerMsg.GetTimeStamp() << "|";
  }
  if ( tLogEntryCfg::eZone & m_logEntryConfig)
  {
    std::cout << rLoggerMsg.GetDBGZone() << "|";
  }
  if ( tLogEntryCfg::eLogEntry & m_logEntryConfig )
  {
    std::cout << rLoggerMsg.GetLogText() << "|";
  }
  if ( tLogEntryCfg::eSource & m_logEntryConfig )
  {
    std::cout << rLoggerMsg.GetSrcFileName() << "|";
  }
  std::cout << std::endl;
}

}
