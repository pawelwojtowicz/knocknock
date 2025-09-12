#pragma once
#include <string>

namespace knocknock
{
class CSHA256Hash
{
  CSHA256Hash() = delete;
  ~CSHA256Hash() = delete;
public:
  static bool CalculateHash( const std::string& message, std::string& stringHex, std::string& rawHex);
};
}