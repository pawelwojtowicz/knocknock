#pragma once
#include "UtilityTypes.h"

namespace knocknock
{
class CTokenizer
{
public:
  static tStringList Tokenize(const std::string& str, char delimiter);

  // Like Tokenize(), but trims whitespace (incl. '\r') around every token and
  // drops empty tokens - meant for user-written lists such as
  // "simpledb, sha256," in the configuration.
  static tStringList TokenizeTrimmed(const std::string& str, char delimiter);

  // Removes leading and trailing whitespace (space, tab, '\r', '\n', ...)
  static std::string Trim(const std::string& str);
private:
  CTokenizer(const CTokenizer&) = delete;
  CTokenizer& operator=(const CTokenizer&) = delete;
};
}