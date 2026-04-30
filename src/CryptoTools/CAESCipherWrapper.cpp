#include "CAESCipherWrapper.h"
#include <openssl/evp.h>
#include <openssl/err.h>
#include <sstream>
#include <iomanip>

namespace knocknock
{

// Helper function to encode bytes to hex string
static std::string hexEncode(const std::string& input) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (unsigned char c : input) {
        ss << std::setw(2) << static_cast<int>(c);
    }
    return ss.str();
}

// Helper function to decode hex string to bytes
static std::string hexDecode(const std::string& input) {
    std::string output;
    output.reserve(input.size() / 2);
    for (size_t i = 0; i < input.size(); i += 2) {
        std::istringstream iss(input.substr(i, 2));
        int byte;
        iss >> std::hex >> byte;
        output.push_back(static_cast<char>(byte));
    }
    return output;
}

// Helper function to get EVP_CIPHER based on key size
static const EVP_CIPHER* getCipher(size_t keySize) {
    if (keySize == 16) return EVP_aes_128_cbc();
    if (keySize == 24) return EVP_aes_192_cbc();
    if (keySize == 32) return EVP_aes_256_cbc();
    return nullptr; // Invalid key size
}

bool CAESCipherWrapper::Encrypt( const std::string& plainText, const std::string& rawIV, const std::string& rawKey, std::string& cipher )
{
    const EVP_CIPHER* cipherType = getCipher(rawKey.size());
    if (!cipherType) return false;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

    if (EVP_EncryptInit_ex(ctx, cipherType, nullptr, reinterpret_cast<const unsigned char*>(rawKey.data()), reinterpret_cast<const unsigned char*>(rawIV.data())) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    std::string ciphertext;
    ciphertext.resize(plainText.size() + EVP_CIPHER_CTX_block_size(ctx));

    int len = 0;
    int ciphertext_len = 0;

    if (EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(&ciphertext[0]), &len, reinterpret_cast<const unsigned char*>(plainText.data()), plainText.size()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    ciphertext_len = len;

    if (EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&ciphertext[0]) + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    ciphertext_len += len;

    ciphertext.resize(ciphertext_len);
    cipher = hexEncode(ciphertext);

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

bool CAESCipherWrapper::EncryptString( const std::string& plainText, const std::string& stringIV, const std::string& stringKey,  std::string& cipherString )
{
    std::string rawIV = hexDecode(stringIV);
    std::string rawKey = hexDecode(stringKey);

    std::string cipherRaw;
    if (Encrypt(plainText, rawIV, rawKey, cipherRaw)) {
        cipherString = cipherRaw; // Already hex encoded in Encrypt
        return true;
    }
    return false;
}
  
bool CAESCipherWrapper::Decrypt( const std::string& cipherString, const std::string& rawIV, const std::string& rawKey ,std::string& plainText )
{
    std::string cipherRaw = hexDecode(cipherString);

    const EVP_CIPHER* cipherType = getCipher(rawKey.size());
    if (!cipherType) return false;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

    if (EVP_DecryptInit_ex(ctx, cipherType, nullptr, reinterpret_cast<const unsigned char*>(rawKey.data()), reinterpret_cast<const unsigned char*>(rawIV.data())) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    std::string plaintext;
    plaintext.resize(cipherRaw.size());

    int len = 0;
    int plaintext_len = 0;

    if (EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(&plaintext[0]), &len, reinterpret_cast<const unsigned char*>(cipherRaw.data()), cipherRaw.size()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    plaintext_len = len;

    if (EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&plaintext[0]) + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    plaintext_len += len;

    plaintext.resize(plaintext_len);
    plainText = plaintext;

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

}