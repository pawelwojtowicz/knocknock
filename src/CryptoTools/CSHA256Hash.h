#pragma once
#include <string>

namespace knocknock
{
class CHashFunction
{
  CHashFunction() = delete;
  ~CHashFunction() = delete;
public:
  static bool CalculateHash( const std::string& message, std::string& stringHex, std::string& rawHex);
};
}