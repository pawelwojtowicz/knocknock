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

static int hexDigitValue(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

// Helper function to decode hex string to bytes. Fails (instead of producing
// garbage bytes) on odd length or any non-hex character.
static bool hexDecode(const std::string& input, std::string& output) {
    if (input.size() % 2 != 0) return false;

    std::string decoded;
    decoded.reserve(input.size() / 2);
    for (size_t i = 0; i < input.size(); i += 2) {
        const int high = hexDigitValue(input[i]);
        const int low = hexDigitValue(input[i + 1]);
        if (high < 0 || low < 0) return false;
        decoded.push_back(static_cast<char>((high << 4) | low));
    }
    output = std::move(decoded);
    return true;
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
    // OpenSSL reads iv_length bytes from the IV pointer - reject short IVs
    if (rawIV.size() < static_cast<size_t>(EVP_CIPHER_iv_length(cipherType))) return false;

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
    std::string rawIV;
    std::string rawKey;
    if (!hexDecode(stringIV, rawIV) || !hexDecode(stringKey, rawKey)) {
        return false;
    }

    std::string cipherRaw;
    if (Encrypt(plainText, rawIV, rawKey, cipherRaw)) {
        cipherString = cipherRaw; // Already hex encoded in Encrypt
        return true;
    }
    return false;
}
  
bool CAESCipherWrapper::Decrypt( const std::string& cipherString, const std::string& rawIV, const std::string& rawKey ,std::string& plainText )
{
    std::string cipherRaw;
    if (!hexDecode(cipherString, cipherRaw)) return false;

    const EVP_CIPHER* cipherType = getCipher(rawKey.size());
    if (!cipherType) return false;
    // OpenSSL reads iv_length bytes from the IV pointer - reject short IVs
    if (rawIV.size() < static_cast<size_t>(EVP_CIPHER_iv_length(cipherType))) return false;

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