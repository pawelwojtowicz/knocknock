#include "CAuthenticationChallengeTools.h"
#include "CSHA256Hash.h"
#include <openssl/rand.h>
#include <sstream>
#include <iomanip>
#include <vector>

namespace knocknock
{

static std::string generateRandomHex(int byteCount)
{
  std::vector<unsigned char> buffer(byteCount);
  RAND_bytes(buffer.data(), byteCount);

  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (unsigned char b : buffer)
  {
    ss << std::setw(2) << static_cast<int>(b);
  }
  return ss.str();
}

std::string CAuthenticationChallengeTools::GenerateAuthenticationChallenge(const std::string& userId)
{
  return generateRandomHex(32);
}

std::string CAuthenticationChallengeTools::GenerateSessionId(const std::string& userId)
{
  std::string rawChallenge = generateRandomHex(32);
  std::string rawBinarySessionId;
  std::string hexEncodedSessionId;

  if (CSHA256Hash::CalculateHash(rawChallenge, rawBinarySessionId, hexEncodedSessionId))
  {
    return hexEncodedSessionId;
  }
  return std::string{};
}

}