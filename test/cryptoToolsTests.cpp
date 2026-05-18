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


