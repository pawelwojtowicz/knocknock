#pragma once
#include "IAuthenticationMethod.h"
#include <string>

namespace knocknock {

class CSession; 

class CSimpleDBAuthMethod : public IAuthenticationMethod {
public:
    virtual ~CSimpleDBAuthMethod() = default;

    virtual std::string Login(CSession& session, const std::string& username, const std::string& password) override;

    virtual std::string Authenticate(CSession& session, const std::string& authenticationPayload) override;
};

} // namespace knock::engine