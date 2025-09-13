#pragma once
#include "IAuthenticationMethod.h"

namespace knocknock {

class CSession; 

// SCR - Simple Challenge Response authentication method
class CSCRAuthMethod : public IAuthenticationMethod {
public:
    virtual ~CSCRAuthMethod() = default;

    virtual tKeyValueMap Login(CSession& session, const std::string& password) override;

    virtual tKeyValueMap Authenticate(CSession& session, const tKeyValueMap& authenticationPayload) override;
};

} // namespace knocknock