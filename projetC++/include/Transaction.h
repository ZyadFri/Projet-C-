#pragma once

#include <string>
#include <ctime>

/*!
 * \brief Represents a transaction within the blockchain.
 */
class Transaction
{
public:
    Transaction(const std::string &from, const std::string &to, double amount);

    // Getters
    std::string getFrom() const { return from; }
    std::string getTo() const { return to; }
    double getAmount() const { return amount; }
    time_t getTimestamp() const { return timestamp; }
    std::string getSignature() const { return signature; }

    // Signing and verification
    void signTransaction(const std::string &privateKey);
    bool verifyTransaction(const std::string &publicKey) const;

    // Hash
    std::string calculateHash() const;

private:
    std::string from; // Typically the sender's public key
    std::string to;   // The receiver's public key
    double amount;
    time_t timestamp;
    std::string signature;
};
