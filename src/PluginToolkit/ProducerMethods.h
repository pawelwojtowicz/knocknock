#pragma once
#include "TBuilderFactory.h"
#include "ISessionPublisher.h"
#include "IAuthenticationMethod.h"

namespace knocknock
{
using tSessionPublisherBuilder = TBuilderFactory<ISessionPublisher>::tCreationMethod;
using tSessionPublisherBuildersMap = TBuilderFactory<ISessionPublisher>::tCreationMethodMap;
using tAuthenticationMethodBuilder = TBuilderFactory<IAuthenticationMethod>::tCreationMethod;
using tAuthenticationMethodBuildersMap = TBuilderFactory<IAuthenticationMethod>::tCreationMethodMap;
}