#pragma once
#include "TBuilderFactory.h"
#include "ISessionPublisher.h"
#include "IAuthenticationMethod.h"

namespace knocknock
{
using tSessionPublisherBuildersMap = TBuilderFactory<ISessionPublisher>::tCreationMethodMap;
using tAuthenticationMethodConstructionFunction = std::function<std::unique_ptr<IAuthenticationMethod>()>;
using tAuthenticationMethodBuildersMap = TBuilderFactory<IAuthenticationMethod>::tCreationMethodMap;
}