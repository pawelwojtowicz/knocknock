#pragma once
#include <IDBAccess.h>
#include "CConfiguration.h"
#include "CSession.h"
#include <optional>

namespace knocknock
{

class CSessionBuilder
{
public:
  CSessionBuilder(CConfiguration& rConfiguration, DBAccess::IDBAccess& rDBAccess);
  virtual ~CSessionBuilder();

  std::optional<CSession> CreateSession(const std::string& userId);

private:
  CConfiguration& m_rConfiguration;

  DBAccess::IDBAccess& m_rDBAccess;

};
}