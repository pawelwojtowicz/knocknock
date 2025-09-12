#include "CSHA256Hash.h"
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

namespace knocknock
{
bool CSHA256Hash::CalculateHash( const std::string& message, std::string& digestRaw, std::string& digestString)
{
  CryptoPP::SHA256 hash;
  hash.Update((CryptoPP::byte*)message.data(), message.size());
  digestRaw.resize(hash.DigestSize());
  hash.Final((CryptoPP::byte*)&digestRaw[0]);

  CryptoPP::HexEncoder hexEncoder;
  hexEncoder.Put((CryptoPP::byte*)digestRaw.data(), digestRaw.size());
  hexEncoder.MessageEnd();

  digestString.resize(hexEncoder.MaxRetrievable());
  hexEncoder.Get((CryptoPP::byte*)&digestString[0], digestString.size() );

  return ( 2* digestRaw.size())  == digestString.size(); 
}
};
