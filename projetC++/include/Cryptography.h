#pragma once

#include <string>
#include <utility>

/*!
 * \brief Cryptographic utilities: hashing, keypair generation, signing, and verification.
 *        Uses OpenSSL.
 */
class Cryptography
{
public:
    static std::string sha256(const std::string &input);

    /*!
     * \brief Generate a key pair (privateKey, publicKey).
     * \return A pair of PEM-encoded strings: (privateKey, publicKey).
     */
    static std::pair<std::string, std::string> generateKeyPair();

    /*!
     * \brief Sign a message using a private key (PEM).
     * \param message     The message to sign.
     * \param privateKey  The PEM-encoded private key.
     * \return Hex-encoded signature.
     */
    static std::string sign(const std::string &message, const std::string &privateKey);

    /*!
     * \brief Verify a signature using a public key (PEM).
     * \param message     The original message.
     * \param signature   The signature (hex-encoded) to verify.
     * \param publicKey   The PEM-encoded public key.
     * \return True if valid, false otherwise.
     */
    static bool verify(const std::string &message, const std::string &signature, const std::string &publicKey);
};
