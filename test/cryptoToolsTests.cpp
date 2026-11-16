#include <gtest/gtest.h>
#include <string.h>
#include <CAESCipherWrapper.h>
#include <CSHA256Hash.h>
#include <CArgon2idWrapper.h>
#include <iostream>

TEST( CSHA256Hash , Base )
{
  std::string hashRaw;
  std::string hashString;
  ASSERT_TRUE(knocknock::CSHA256Hash::CalculateHash("Input", hashRaw, hashString));
  for ( int i = 0 ; i < hashRaw.size() ; ++i)
  {
    std::string byteString = hashString.substr(i*2, 2);
    char hexFromString = std::stoi(byteString,0,16);
    EXPECT_EQ( hashRaw[i], hexFromString);
  };
}


TEST( CAESCipherWrapper, Basic)
{
  std::string ivString("InitString");
  std::string keyString("ThePasswordString");

  std::string ivHashString;
  std::string ivHashRaw;
  std::string keyHashString;
  std::string keyHashRaw;

  knocknock::CSHA256Hash::CalculateHash(ivString, ivHashRaw, ivHashString);
  knocknock::CSHA256Hash::CalculateHash(keyString, keyHashRaw, keyHashString );

  std::string planTextToEncrypt("This is a test string, that will be encrypted");

  std::string cipher;
  knocknock::CAESCipherWrapper::Encrypt(planTextToEncrypt, ivHashRaw, keyHashRaw,cipher);

  std::string plainDecrypted = {};
  knocknock::CAESCipherWrapper::Decrypt(cipher, ivHashRaw, keyHashRaw, plainDecrypted);

  EXPECT_EQ(planTextToEncrypt, plainDecrypted);

}

TEST( CAESCipherWrapper, EncryptString_MalformedHexRejected)
{
  const std::string validIV(32, 'a');   // 16 bytes
  const std::string validKey(64, 'b');  // 32 bytes
  std::string cipher;

  ASSERT_TRUE(knocknock::CAESCipherWrapper::EncryptString("text", validIV, validKey, cipher));

  // odd length
  EXPECT_FALSE(knocknock::CAESCipherWrapper::EncryptString("text", validIV, validKey.substr(1), cipher));
  // non-hex characters
  EXPECT_FALSE(knocknock::CAESCipherWrapper::EncryptString("text", validIV, std::string(64, 'z'), cipher));
  EXPECT_FALSE(knocknock::CAESCipherWrapper::EncryptString("text", std::string(32, 'g'), validKey, cipher));
}

TEST( CAESCipherWrapper, ShortIVRejected)
{
  const std::string shortIV(8, 'a');    // 4 bytes, AES needs 16
  const std::string validKey(64, 'b');
  std::string cipher;

  EXPECT_FALSE(knocknock::CAESCipherWrapper::EncryptString("text", shortIV, validKey, cipher));

  std::string plain;
  EXPECT_FALSE(knocknock::CAESCipherWrapper::Decrypt("00", std::string(4, 'x'), std::string(32, 'k'), plain));
  // malformed cipher hex
  EXPECT_FALSE(knocknock::CAESCipherWrapper::Decrypt("0", std::string(16, 'x'), std::string(32, 'k'), plain));
}

TEST( CArgon2idWrapper, Basic_InOut)
{
  std::string password("ThisIsThePassword");
  std::string hash;
  ASSERT_TRUE(CryptoTools::CArgon2idWrapper::HashPassword(password, 0, 0, 0, 0, hash));
  ASSERT_TRUE( CryptoTools::CArgon2idWrapper::VerifyPassword( password, hash));
}

TEST( CArgon2idWrapper, OnlineExample)
{
  std::string password("secretWord$123");
  std::string hash("$argon2id$v=19$m=16,t=4,p=2$NnFHeTJ2ckNXMFdKVENWVw$VhR9NKJHJ+pT7DVTqE7ksA");
  ASSERT_TRUE( CryptoTools::CArgon2idWrapper::VerifyPassword( password, hash));
}


