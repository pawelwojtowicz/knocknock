#pragma once
#include "IURLMatcher.h"

namespace HTTPServer 
{
class CSimpleMatcher : public IURLMatcher
{
public:
  CSimpleMatcher( const std::string& urlPattern );
  virtual ~CSimpleMatcher() = default;

  bool PathMatches( const std::string& url ) override;

private:
  std::string m_urlPattern;

  size_t m_patternSize;
};
}