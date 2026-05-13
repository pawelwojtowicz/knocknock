#pragma once
#include <string>
#include <argon2.h>

namespace CryptoTools
{
class CArgon2idWrapper
{
public:
static bool HashPassword(const std::string& password, uint32_t timeCost, uint32_t memoryCost, uint32_t parallelism, uint32_t hashLength, std::string& outputHash);
static bool VerifyPassword(const std::string& password, const std::string& expectedHash);
};
}