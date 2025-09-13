#include "CAESCipherWrapper.h"
#include <cryptopp/cryptlib.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/hex.h>



using namespace CryptoPP;

namespace knocknock
{

bool CAESCipherWrapper::Encrypt( const std::string& plainText, const std::string& rawIV, const std::string& rawKey, std::string& cipher )
{
  CBC_Mode< AES >::Encryption encryptor;
  encryptor.SetKeyWithIV((byte*)rawKey.data(),rawKey.size(),(byte*)rawIV.data());

  StringSource ss( plainText, true, 
        new StreamTransformationFilter( encryptor,
           new HexEncoder(
              new StringSink( cipher )
           ) // HexEncoder
	      ) // StreamTransformationFilter      
  ); // StringSource

  return true;
}

bool CAESCipherWrapper::EncryptString( const std::string& plainText, const std::string& stringIV, const std::string& stringKey,  std::string& cipherString )
{
  std::string rawIV{};
  std::string rawKey{};

  {
  CryptoPP::StringSource ss(
            stringIV,
            true,
            new CryptoPP::HexDecoder(
                new CryptoPP::StringSink(rawIV)
            )
        );
  }

  {
    CryptoPP::StringSource ss(
            stringKey,
            true,
            new CryptoPP::HexDecoder(
                new CryptoPP::StringSink(rawKey)
            )
        );
  }


  std::string cipherRaw{};
  if (Encrypt(plainText, rawIV, rawKey, cipherRaw))
  {
    CryptoPP::HexEncoder hexEncoder;
    hexEncoder.Put((CryptoPP::byte*)cipherRaw.data(), cipherRaw.size());
    hexEncoder.MessageEnd();

    cipherString.resize(hexEncoder.MaxRetrievable());
    hexEncoder.Get((CryptoPP::byte*)&cipherString[0], cipherString.size() );

    return true;
  }
  return false;
}
  
bool CAESCipherWrapper::Decrypt( const std::string& cipherString, const std::string& rawIV, const std::string& rawKey ,std::string& plainText )
{
  CryptoPP::HexDecoder hexDecoder;
  hexDecoder.Put((CryptoPP::byte*)cipherString.data(), cipherString.size());
  hexDecoder.MessageEnd();

  std::string cipherRaw;
  cipherRaw.resize(hexDecoder.MaxRetrievable());
  hexDecoder.Get((byte*)&cipherRaw[0],cipherRaw.size());

  CBC_Mode< AES >::Decryption decryptor;
  decryptor.SetKeyWithIV((byte*)rawKey.data(),rawKey.size(),(byte*)rawIV.data());

  StringSource s(cipherRaw, true, 
    new StreamTransformationFilter(decryptor,
      new StringSink(plainText)
      ) // StreamTransformationFilter
  ); // StringSource
  return true;
}


}