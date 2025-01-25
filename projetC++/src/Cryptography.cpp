#include "Cryptography.h"
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>

std::string Cryptography::sha256(const std::string &input)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.size());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::pair<std::string, std::string> Cryptography::generateKeyPair()
{
    // Generate an RSA key in memory (2048 bits)
    const int kBits = 2048;
    const unsigned long kExp = RSA_F4;
    RSA *r = RSA_generate_key(kBits, kExp, nullptr, nullptr);
    if (!r)
    {
        throw std::runtime_error("RSA_generate_key failed.");
    }

    // Convert private key to PEM
    BIO *pri = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPrivateKey(pri, r, nullptr, nullptr, 0, nullptr, nullptr);

    char *pri_pem = nullptr;
    long pri_len = BIO_get_mem_data(pri, &pri_pem);
    std::string privateKey(pri_pem, pri_len);

    // Convert public key to PEM
    BIO *pub = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pub, r);

    char *pub_pem = nullptr;
    long pub_len = BIO_get_mem_data(pub, &pub_pem);
    std::string publicKey(pub_pem, pub_len);

    BIO_free_all(pub);
    BIO_free_all(pri);
    RSA_free(r);

    return std::make_pair(privateKey, publicKey);
}

std::string Cryptography::sign(const std::string &message, const std::string &privateKey)
{
    // Convert privateKey string to BIO
    BIO *bio = BIO_new_mem_buf((void *)privateKey.data(), (int)privateKey.size());
    if (!bio)
    {
        throw std::runtime_error("Failed to create BIO for private key.");
    }

    // Read the private key
    RSA *rsa = PEM_read_bio_RSAPrivateKey(bio, nullptr, nullptr, nullptr);
    if (!rsa)
    {
        BIO_free(bio);
        throw std::runtime_error("PEM_read_bio_RSAPrivateKey failed.");
    }

    // Hash the message
    std::string hashVal = sha256(message);

    // Sign with RSA
    std::vector<unsigned char> sigBuf(RSA_size(rsa));
    unsigned int sigLen = 0;

    if (RSA_sign(NID_sha256,
                 reinterpret_cast<const unsigned char *>(hashVal.c_str()),
                 (unsigned int)hashVal.size(),
                 sigBuf.data(),
                 &sigLen,
                 rsa) != 1)
    {
        RSA_free(rsa);
        BIO_free(bio);
        throw std::runtime_error("RSA_sign failed.");
    }

    // Convert signature to hex
    std::stringstream ss;
    for (unsigned int i = 0; i < sigLen; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)sigBuf[i];
    }

    RSA_free(rsa);
    BIO_free(bio);

    return ss.str();
}

bool Cryptography::verify(const std::string &message, const std::string &signature, const std::string &publicKey)
{
    // Convert publicKey string to BIO
    BIO *bio = BIO_new_mem_buf((void *)publicKey.data(), (int)publicKey.size());
    if (!bio)
    {
        throw std::runtime_error("Failed to create BIO for public key.");
    }

    // Read the public key
    RSA *rsa = PEM_read_bio_RSAPublicKey(bio, nullptr, nullptr, nullptr);
    if (!rsa)
    {
        BIO_free(bio);
        return false;
    }

    // Hash the message
    std::string hashVal = sha256(message);

    // Convert signature from hex to bytes
    std::vector<unsigned char> sigBytes;
    sigBytes.reserve(signature.size() / 2);
    for (size_t i = 0; i < signature.size(); i += 2)
    {
        std::string byteString = signature.substr(i, 2);
        unsigned char byte = (unsigned char)strtol(byteString.c_str(), nullptr, 16);
        sigBytes.push_back(byte);
    }

    // Verify signature
    bool result = (RSA_verify(NID_sha256,
                              reinterpret_cast<const unsigned char *>(hashVal.c_str()),
                              (unsigned int)hashVal.size(),
                              sigBytes.data(),
                              (unsigned int)sigBytes.size(),
                              rsa) == 1);

    RSA_free(rsa);
    BIO_free(bio);

    return result;
}
