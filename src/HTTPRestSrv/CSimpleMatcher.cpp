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
  return ( 0 == url.find(m_urlPattern ) );
}
}