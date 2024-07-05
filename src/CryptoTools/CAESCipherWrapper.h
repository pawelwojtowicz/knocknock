#pragma once
#include <string>

namespace knocknock
{

class CAESCipherWrapper
{
  CAESCipherWrapper() = delete;
  ~CAESCipherWrapper() = delete;
public:
  static bool Encrypt( const std::string& plainText, const std::string& rawIV, const std::string& rawKey,  std::string& cipher );
  
  static bool Decrypt( const std::string& cipher, const std::string& rawIV, const std::string& rawKey,  std::string& plainText );
};

}