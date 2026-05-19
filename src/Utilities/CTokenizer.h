#pragma once
#include "UtilityTypes.h"

namespace knocknock
{
class CTokenizer
{
public:
  static tStringList Tokenize(const std::string& str, char delimiter);
private:
  CTokenizer(const CTokenizer&) = delete;
  CTokenizer& operator=(const CTokenizer&) = delete;
};
}