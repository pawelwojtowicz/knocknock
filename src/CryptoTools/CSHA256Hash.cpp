#include "CSHA256Hash.h"
#include <openssl/evp.h>
#include <sstream>
#include <iomanip>

namespace knocknock
{

static std::string hexEncode(const std::string& input) {
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0');
    for (unsigned char c : input) {
        ss << std::setw(2) << static_cast<int>(c);
    }
    return ss.str();
}

bool CSHA256Hash::CalculateHash( const std::string& message, std::string& rawBinary, std::string& hexString)
{
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx)
    {
        return false;
    }

    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1)
    {
        EVP_MD_CTX_free(ctx);
        return false;
    }

    if (EVP_DigestUpdate(ctx, message.data(), message.size()) != 1)
    {
        EVP_MD_CTX_free(ctx);
        return false;
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen = 0;

    if (EVP_DigestFinal_ex(ctx, hash, &hashLen) != 1)
    {
        EVP_MD_CTX_free(ctx);
        return false;
    }

    EVP_MD_CTX_free(ctx);

    rawBinary = std::string(reinterpret_cast<char*>(hash), hashLen);
    hexString = hexEncode(rawBinary);

    return true;
}

}
