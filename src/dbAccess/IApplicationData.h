#pragma once
#include <string>
#include <optional>
#include "CApplication.h"

namespace DBAccess
{
class IApplicationData
{
public:
    virtual ~IApplicationData() = default;

    virtual bool AddApplication(const knocknock::CApplication& applicationRecord) = 0;
    virtual bool UpdateApplication(const knocknock::CApplication& applicationRecord) = 0;
    virtual bool DeleteApplication(int appId) = 0;
    virtual std::optional<knocknock::CApplication> GetApplication(int appId) = 0;
    virtual knocknock::tApplicationArray GetAllApplications() = 0;
};
}
