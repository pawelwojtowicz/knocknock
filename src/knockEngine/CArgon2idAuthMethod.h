#pragma once
#include "IAuthenticationMethod.h"

namespace knocknock {

class CSession;

class CArgon2idAuthMethod : public IAuthenticationMethod {
public:
    virtual ~CArgon2idAuthMethod() = default;

    virtual tKeyValueMap Login(CSession& session, const tKeyValueMap& loginPayload) override;

    virtual tKeyValueMap Authenticate(CSession& session, const tKeyValueMap& authenticationPayload) override;
};

} // namespace knocknock
