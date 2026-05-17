#pragma once
#include <string>


namespace knocknock
{
class CSession;
class CConfiguration;

class CCookieBuilder
{
public:
  CCookieBuilder() = default;
  ~CCookieBuilder() = default;

  bool Initialize( const CConfiguration& config );

  std::string BuildCookie( const CSession& session ) const;

private:
  std::string m_cookiePostfix = {""};
};
}