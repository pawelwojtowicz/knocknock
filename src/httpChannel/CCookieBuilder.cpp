#include "CCookieBuilder.h"
#include <CSession.h>
#include <CConfiguration.h>
#include "httpChannelConst.h"
#include <algorithm>
namespace knocknock
{
bool CCookieBuilder::Initialize( const CConfiguration& config )
{  
  constexpr char cParamNameHTTPCookieHttpOnly[] = "http.cookieHttpOnly";
  constexpr char cParamNameHTTPCookiePath[] = "http.cookiePath";
  constexpr char cParamNameHTTPCookieSecure[] = "http.cookieSecure";
  constexpr char cParamNameHTTPCookieSameSite[] = "http.cookieSameSite";



  std::string paramValueString = config.GetParamString(cParamNameHTTPCookieHttpOnly, "true") ;
  if (paramValueString == "true" || paramValueString == "1" || paramValueString == "TRUE")
  {
    m_cookiePostfix = "; HttpOnly";
  }
  
  paramValueString = config.GetParamString(cParamNameHTTPCookiePath, "/" );
  m_cookiePostfix += "; Path=" + paramValueString;

  paramValueString = config.GetParamString(cParamNameHTTPCookieSecure, "false");
  if  (paramValueString == "true" || paramValueString == "1" || paramValueString == "TRUE")
  {
    m_cookiePostfix += "; Secure";
  } 

  paramValueString = config.GetParamString(cParamNameHTTPCookieSameSite, "Lax");
  std::transform(paramValueString.begin(), paramValueString.end(), paramValueString.begin(), ::tolower);
  if ( paramValueString == "lax" )
  {
    m_cookiePostfix += "; SameSite=Lax";
  }
  else if ( paramValueString == "strict" )
  {
    m_cookiePostfix += "; SameSite=Strict";
  }
  else if ( paramValueString == "none" )
  {
    m_cookiePostfix += "; SameSite=None";
  }

   return true;
}

std::string CCookieBuilder::BuildCookie( const CSession& session ) const
{
  std::string cookie = "sid=" + session.GetSessionId() ;
  cookie += "; Max-Age=" + std::to_string( session.GetMaxAge() ) + m_cookiePostfix;

  return cookie;
}

} // namespace knocknock