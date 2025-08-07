#pragma once
#include <string>
#include "CPrivilege.h"

namespace DBAccess
{
class IRole2PrivilegeMappingData
{
public:
    virtual ~IRole2PrivilegeMappingData() = default;

    virtual bool AddPrivilege2Role(const std::string& roleName, const std::string& privileShortDesc) = 0;
    virtual bool RemovePrivilegeFromRole(const std::string& roleName, const std::string& privileShortDesc) = 0;
    virtual knocknock::tPrivilegeArray GetPrivilegesForRole(const std::string& roleName) = 0;
};
}
