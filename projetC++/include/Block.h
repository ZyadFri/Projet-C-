#pragma once

#include <string>
#include <vector>
#include <ctime>
#include "Transaction.h"

/*!
 * \brief Represents a single block in the blockchain.
 */
class Block
{
public:
    // Constructors
    Block(uint32_t indexIn, const std::string &dataIn);
    Block(uint32_t indexIn, const std::vector<Transaction> &transactionsIn);

    // Getters
    uint32_t getIndex() const { return index; }
    const std::string &getHash() const { return hash; }
    const std::string &getPreviousHash() const { return previousHash; }
    const std::string &getData() const { return data; }
    const std::vector<Transaction> &getTransactions() const { return transactions; }
    time_t getTimestamp() const { return timestamp; }
    uint64_t getNonce() const { return nonce; }

    // Setters
    void setPreviousHash(const std::string &hash) { previousHash = hash; }
    void setNonce(uint64_t nonceIn) { nonce = nonceIn; }

    // Mining
    void mineBlock(uint32_t difficulty);
    bool isValid(uint32_t difficulty) const;

    // Serialization
    std::string serialize() const;
    static Block deserialize(const std::string &serialized);

private:
    uint32_t index;
    std::string data;
    std::string hash;
    std::string previousHash;
    std::vector<Transaction> transactions;
    time_t timestamp;
    uint64_t nonce;

    std::string calculateHash() const;
};
