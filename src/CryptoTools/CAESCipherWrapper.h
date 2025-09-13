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
  static bool EncryptString( const std::string& plainText, const std::string& stringIV, const std::string& stringKey,  std::string& cipherString );
  
  static bool Decrypt( const std::string& cipher, const std::string& rawIV, const std::string& rawKey,  std::string& plainText );
};

}