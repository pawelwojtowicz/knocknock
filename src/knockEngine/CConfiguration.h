#pragma once
#include <string>
#include <IDBAccess.h>
#include "knocknockTypes.h"

namespace knocknock
{
  class CConfiguration
  {
  public:
    CConfiguration() = default;
    ~CConfiguration() = default;


    bool LoadConfig(DBAccess::IDBAccess& rDBAccess);
    bool LoadConfig(const std::string& filename);

    const std::string& GetAnonymousUserTemplate() const { return m_anonumousUserTemplate; }

    const std::string& GetDefaultAuthenticationMethod() const { return m_defaultAuthenticationMethod; }
    const std::string& GetDefaultAuthenticationString() const { return m_defaultAuthenticationString; }

    const std::string& GetAllowedAuthMethods() const { return m_allowedAuthMethods; }

  private:
    std::string m_anonumousUserTemplate = "";

    std::string m_defaultAuthenticationMethod = "sha256";
    std::string m_defaultAuthenticationString = "db89a15ca72c6c91a94c03e6b7973bbbf01b3e67988c9f79d6b764b36d913a66";

    std::string m_allowedAuthMethods = "simpledb,sha256,scr";

    int m_sessionMaxAge = 0;
    bool m_cookieHttpOnly = false;
    std::string m_cookiePath = "/";
    bool m_cookieSecure = false;
    CookieSameSite m_cookieSameSite = CookieSameSite::LAX;

  };
}