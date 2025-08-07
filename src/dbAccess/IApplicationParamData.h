#pragma once
#include <string>
#include <optional>
#include "CApplicationParam.h"

namespace DBAccess
{
class IApplicationParamData
{
public:
    virtual ~IApplicationParamData() = default;

    virtual bool AddApplicationParam(const knocknock::CApplicationParam& applicationParam) = 0;
    virtual bool UpdateApplicationParam(const knocknock::CApplicationParam& applicationParam) = 0;
    virtual std::optional<knocknock::CApplicationParam> GetApplicationParam(int applicationId, const std::string& paramName) = 0;
    virtual knocknock::tApplicationParamsArray GetApplicationParams(int applicationId) = 0;
    virtual knocknock::tApplicationParamsArray GetAllParams() = 0;
    virtual bool DeleteApplicationParams(int applicationId) = 0;
    virtual bool DeleteApplicationParameter(int applicationId, const std::string& paramName) = 0;
};
}
