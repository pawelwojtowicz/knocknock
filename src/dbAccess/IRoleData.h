#pragma once
#include <string>
#include <vector>
#include <optional>
#include "CRole.h"

namespace DBAccess
{
using tRoleList = std::vector<knocknock::CRole>;

class IRoleData
{
public:
    virtual ~IRoleData() = default;

    // CRUD operations for roles
    virtual bool AddRole( const knocknock::CRole& role) = 0;
    virtual bool DeleteRole(const std::string& roleName) = 0;
    virtual bool UpdateRole(const knocknock::CRole& role) = 0;
    virtual knocknock::tRoles GetAllRoles() const = 0;
    virtual std::optional<knocknock::CRole> GetRole(const std::string& roleName) const = 0;
};

}
