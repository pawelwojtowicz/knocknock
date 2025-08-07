#pragma once
#include "IApplicationData.h"
namespace DBAccess
{
class IDBDriver;

class CApplicationData : public IApplicationData
{
public:
  CApplicationData(IDBDriver& rDBDriver);
  ~CApplicationData() = default;

  bool AddApplication( const knocknock::CApplication& applicationRecord) override;
  bool UpdateApplication( const knocknock::CApplication& applicationRecord ) override;
  bool DeleteApplication( const int appId ) override;

  std::optional<knocknock::CApplication> GetApplication( const int appId) override;
  knocknock::tApplicationArray GetAllApplications() override;

private:
  IDBDriver& m_rDBDriver;
};
}