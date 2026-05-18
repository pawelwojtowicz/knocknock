#include "CArgon2idWrapper.h"
#include <openssl/rand.h>

namespace CryptoTools
{
constexpr uint32_t DEFAULT_TIME_COST = 2; // Number of iterations
constexpr uint32_t DEFAULT_MEMORY_COST = 1 << 16; // Memory usage in KiB (64 MiB)
constexpr uint32_t DEFAULT_PARALLELISM = 1; // Number of parallel threads
constexpr uint32_t DEFAULT_HASH_LENGTH = 32; // Length of the resulting hash in bytes
constexpr uint32_t SALT_LENGTH = 16; // Length of the salt in bytes

bool CArgon2idWrapper::HashPassword(const std::string& password, uint32_t timeCost, uint32_t memoryCost, uint32_t parallelism, uint32_t hashLength, std::string& outputHash)
{
  if (timeCost == 0) timeCost = DEFAULT_TIME_COST;
  if (memoryCost == 0) memoryCost = DEFAULT_MEMORY_COST;
  if (parallelism == 0) parallelism = DEFAULT_PARALLELISM;
  if (hashLength == 0) hashLength = DEFAULT_HASH_LENGTH;

  uint8_t salt[SALT_LENGTH];
  if (RAND_bytes(salt, SALT_LENGTH) != 1)
  {
    return false;
  }

  size_t encodedLen = argon2_encodedlen(timeCost, memoryCost, parallelism, hashLength, SALT_LENGTH, Argon2_id);
  if (encodedLen == 0)
  {
    return false;
  }

  std::string encoded(encodedLen, '\0');
  int result = argon2id_hash_encoded(timeCost, memoryCost, parallelism,
                                     password.c_str(), password.size(),
                                     salt, SALT_LENGTH,
                                     hashLength,
                                     encoded.data(), encodedLen);
  if (result != ARGON2_OK)
  {
    return false;
  }

  outputHash.assign(encoded.c_str(), encoded.size());
  return true;
}

bool CArgon2idWrapper::VerifyPassword(const std::string& password, const std::string& expectedHash)
{
  int result = argon2id_verify(expectedHash.c_str(), password.c_str(), password.size());
  return (result == ARGON2_OK);
}

}