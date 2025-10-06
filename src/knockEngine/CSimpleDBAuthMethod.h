#pragma once
#include "IAuthenticationMethod.h"
#include <string>

namespace knocknock {

class CSession; 

class CSimpleDBAuthMethod : public IAuthenticationMethod {
public:
    virtual ~CSimpleDBAuthMethod() = default;

    virtual tKeyValueMap Login(CSession& session, const tKeyValueMap& loginPayload) override;

    virtual tKeyValueMap Authenticate(CSession& session, const tKeyValueMap& authenticationPayload) override;
};

} // namespace knocknock