#include "CSHA256Hash.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

namespace knocknock
{

// Helper function to encode bytes to hex string
static std::string hexEncode(const std::string& input) {
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0');
    for (unsigned char c : input) {
        ss << std::setw(2) << static_cast<int>(c);
    }
    return ss.str();
}

bool CSHA256Hash::CalculateHash( const std::string& message, std::string& rawHex, std::string& stringHex)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, reinterpret_cast<const unsigned char*>(message.data()), message.size());
    SHA256_Final(hash, &sha256);

    rawHex = std::string(reinterpret_cast<char*>(hash), SHA256_DIGEST_LENGTH);
    stringHex = hexEncode(rawHex);

    return true;
}

}
