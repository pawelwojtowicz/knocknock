#pragma once
#include <string>

namespace knocknock {

class CSession; 

class IAuthenticationMethod {
public:
    virtual ~IAuthenticationMethod() = default;

    virtual std::string Login(CSession& session, const std::string& username, const std::string& password) = 0;
    virtual std::string Authenticate(CSession& session, const std::string& authenticationPayload) = 0;
};

} // namespace knock::engine