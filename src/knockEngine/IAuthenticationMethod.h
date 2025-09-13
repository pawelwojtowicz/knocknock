#pragma once
#include <string>
#include "knocknockTypes.h"

namespace knocknock 
{

class CSession; 

class IAuthenticationMethod {
public:
    virtual ~IAuthenticationMethod() = default;

    virtual tKeyValueMap Login(CSession& session, const std::string& password) = 0;
    virtual tKeyValueMap Authenticate(CSession& session, const tKeyValueMap& authenticationPayload) = 0;
};

} // namespace knocknock