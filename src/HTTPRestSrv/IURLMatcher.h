#pragma once
#include <string>

namespace HTTPServer
{

class IURLMatcher
{
public:
  IURLMatcher() = default;
  virtual ~IURLMatcher() = default;

  virtual bool PathMatches( const std::string& url ) = 0;

};
}