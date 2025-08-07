#pragma once
#include <string>
#include "CRole.h"
#include "CPrivilege.h"

namespace DBAccess
{

class IUser2RoleMappingData
{
public:
    virtual ~IUser2RoleMappingData() = default;

    virtual bool AssignRoleToUser(const std::string& userId, const std::string& roleName) = 0;
    virtual bool RemoveRoleFromUser(const std::string& userId, const std::string& roleName) = 0;
    virtual knocknock::tRoles GetUserRoles(const std::string& userId) = 0;
    virtual knocknock::tPrivilegeArray GetUserPrivileges(const std::string& userId) = 0;
};

}
