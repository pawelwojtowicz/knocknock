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

  bool Initialize();

  std::optional<CSession> CreateSession(const std::string& userId);

private:
  CConfiguration& m_rConfiguration;

  DBAccess::IDBAccess& m_rDBAccess;

  std::string m_defaultAuthMethod;
  std::string m_defaultAuthString;
  std::string m_anonymousTemplate;
};
}