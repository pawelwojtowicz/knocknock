#pragma once

namespace knocknock
{
constexpr char sLoginUserId[] = "userId";
constexpr char sLoginPassword[] = "password";
constexpr char sLoginSessionId[] = "sessionId";

constexpr auto cParamName_AnonymousUserTemplate = "anonymousUserTemplate";
constexpr auto cParamName_DefaultAuthenticationMethod = "defaultAuthenticationMethod";
constexpr auto cParamName_DefaultAuthenticationString = "defaultAuthenticationString";
constexpr auto cParamName_AllowedAuthMethods = "allowedAuthMethods";

constexpr auto cParamValue_AnonymousUserTemplate = "";
constexpr auto cParamValue_DefaultAuthenticationMethod = "sha256";
constexpr auto cParamValue_DefaultAuthenticationString = "db89a15ca72c6c91a94c03e6b7973bbbf01b3e67988c9f79d6b764b36d913a66";
constexpr auto cParamValue_AllowedAuthMethods = "simpledb,sha256,scr";


}