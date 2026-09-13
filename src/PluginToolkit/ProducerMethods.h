#pragma once
#include <functional>
#include <memory>
#include <map>
#include "ISessionPublisher.h"
#include "IAuthenticationMethod.h"

namespace knocknock
{
using tSessionPublisherConstructionFunction = std::function<std::unique_ptr<ISessionPublisher>()>;
using tSessionPublisherBuildersMap = std::map<std::string, tSessionPublisherConstructionFunction>;
using tAuthenticationMethodConstructionFunction = std::function<std::unique_ptr<IAuthenticationMethod>()>;
using tAuthenticationMethodBuildersMap = std::map<std::string, tAuthenticationMethodConstructionFunction>;
}