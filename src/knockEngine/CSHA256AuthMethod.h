#pragma once
#include "IAuthenticationMethod.h"

namespace knocknock {

class CSession; 

class CSHA256AuthMethod : public IAuthenticationMethod {
public:
    virtual ~CSHA256AuthMethod() = default;

    virtual tKeyValueMap Login(CSession& session, const std::string& password) override;

    virtual tKeyValueMap Authenticate(CSession& session, const tKeyValueMap& authenticationPayload) override;
};

} // namespace knocknock