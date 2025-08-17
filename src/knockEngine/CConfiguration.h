#pragma once
#include <string>

namespace knocnock
{
  class CConfiguration
  {
  public:
    CConfiguration() = default;
    ~CConfiguration() = default;

    bool LoadConfig(const std::string& filename);
 
  private:
    bool m_allowAnonymousLogin = false;

    std::string m_defaultAuthentication = "sha256";
  };
}