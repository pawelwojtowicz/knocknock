#pragma once
#include <CApplication.h>
#include <optional>

namespace DBAccess
{
class IDBDriver;

class CApplicationData
{
public:
  CApplicationData(IDBDriver& rDBDriver);
  ~CApplicationData() = default;

  bool AddApplication( const knocknock::CApplication& applicationRecord);
  bool UpdateApplication( const knocknock::CApplication& applicationRecord );
  bool DeleteApplication( const int appId );

  std::optional<knocknock::CApplication> GetApplication( const int appId);
  knocknock::tApplicationArray GetAllApplications();  

private:
  IDBDriver& m_rDBDriver;
};
}