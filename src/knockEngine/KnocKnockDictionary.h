#pragma once
#include <cstdint>

namespace knocknock
{
constexpr char sLoginUserId[] = "userId";
constexpr char sLoginPassword[] = "password";
constexpr char sLoginSessionId[] = "sessionId";

constexpr auto cParamName_AnonymousUserTemplate = "anonymousUserTemplate";
constexpr auto cParamName_DefaultAuthenticationMethod = "defaultAuthenticationMethod";
constexpr auto cParamName_DefaultAuthenticationString = "defaultAuthenticationString";
constexpr auto cParamName_AllowedAuthMethods = "allowedAuthMethods";
constexpr auto cParamName_MaxLoginAttempts = "maxLoginAttempts";
constexpr auto cParamName_LoginLockoutSeconds = "loginLockoutSeconds";

constexpr auto cParamValue_AnonymousUserTemplate = "";
constexpr auto cParamValue_DefaultAuthenticationMethod = "";
constexpr auto cParamValue_DefaultAuthenticationString = "";
constexpr auto cParamValue_AllowedAuthMethods = "simpledb,sha256,scr";

constexpr int cParamValue_MaxLoginAttempts = 5;
constexpr int64_t cParamValue_LoginLockoutSeconds = 30;


}