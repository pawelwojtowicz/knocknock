#pragma once
#include <string>

namespace knocknock
{
  class CConfiguration
  {
  public:
    CConfiguration() = default;
    ~CConfiguration() = default;

    bool LoadConfig(const std::string& filename);

    bool GetAllowAnonymousLogin() const { return m_allowAnonymousLogin; }
    std::string GetDefaultAuthenticationMethod() const { return m_defaultAuthenticationMethod; }
    std::string GetDefaultAuthenticationString() const { return m_defaultAuthenticationString; }

  private:
    bool m_allowAnonymousLogin = false;

    std::string m_defaultAuthenticationMethod = "sha256";
    std::string m_defaultAuthenticationString = "db89a15ca72c6c91a94c03e6b7973bbbf01b3e67988c9f79d6b764b36d913a66";
  };
}