#pragma once
#include "IAuthenticationMethod.h"
#include <string>

namespace knocknock {

class CSession; 

class CSHA256AuthMethod : public IAuthenticationMethod {
public:
    virtual ~CSHA256AuthMethod() = default;

    virtual std::string Login(CSession& session, const std::string& username, const std::string& password) override;

    virtual std::string Authenticate(CSession& session, const std::string& authenticationPayload) override;
};

} // namespace knocknock