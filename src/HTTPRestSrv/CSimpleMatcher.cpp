#include "CSimpleMatcher.h"

namespace HTTPServer 
{

CSimpleMatcher::CSimpleMatcher( const std::string& urlPattern )
: m_urlPattern( urlPattern )
, m_patternSize( urlPattern.size() )
{ 
}

bool CSimpleMatcher::PathMatches( const std::string& url )
{
  if ( 0 != url.find(m_urlPattern) )
  {
    return false;
  }
  // If the URL is longer than the pattern and the pattern doesn't end with '/',
  // ensure the match is at a path boundary (next char must be '/', '?', or '#')
  // e.g. pattern "/login" should not match "/login_admin" but should match "/login?foo=bar"
  if ( url.size() > m_patternSize && m_urlPattern.back() != '/' )
  {
    char nextChar = url[m_patternSize];
    if ( nextChar != '/' && nextChar != '?' && nextChar != '#' )
    {
      return false;
    }
  }
  return true;
}
}