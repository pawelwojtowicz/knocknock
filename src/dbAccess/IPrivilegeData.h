#pragma once
#include <string>
#include <optional>
#include "CPrivilege.h"

namespace DBAccess
{
class IPrivilegeData
{
public:
    virtual ~IPrivilegeData() = default;

    virtual bool AddPrivilege(const knocknock::CPrivilege& privilege) = 0;
    virtual bool UpdatePrivilege(const knocknock::CPrivilege& privilege) = 0;
    virtual bool DeletePrivilege(const std::string& shortDesc) = 0;
    virtual std::optional<knocknock::CPrivilege> GetPrivilege(const std::string& shortDesc) = 0;
    virtual knocknock::tPrivilegeArray GetAllPrivileges() = 0;
};
}
