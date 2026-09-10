#include "CAuthenticationChallengeTools.h"
#include "CSHA256Hash.h"
#include <openssl/rand.h>
#include <sstream>
#include <iomanip>
#include <vector>

namespace knocknock
{

static bool generateRandomHex(int byteCount, std::string& output)
{
  std::vector<unsigned char> buffer(byteCount);
  if (RAND_bytes(buffer.data(), byteCount) != 1)
  {
    return false;
  }

  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (unsigned char b : buffer)
  {
    ss << std::setw(2) << static_cast<int>(b);
  }
  output = ss.str();
  return true;
}

std::string CAuthenticationChallengeTools::GenerateAuthenticationChallenge(const std::string& userId)
{
  std::string challenge;
  if (!generateRandomHex(32, challenge))
  {
    return std::string{};
  }
  return challenge;
}

std::string CAuthenticationChallengeTools::GenerateSessionId(const std::string& userId)
{
  std::string rawChallenge;
  if (!generateRandomHex(32, rawChallenge))
  {
    return std::string{};
  }
  std::string rawBinarySessionId;
  std::string hexEncodedSessionId;

  if (CSHA256Hash::CalculateHash(rawChallenge, rawBinarySessionId, hexEncodedSessionId))
  {
    return hexEncodedSessionId;
  }
  return std::string{};
}

}