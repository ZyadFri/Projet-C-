#include "Block.h"
#include "Cryptography.h"
#include <sstream>
#include <json.hpp>

using json = nlohmann::json;

Block::Block(uint32_t indexIn, const std::string &dataIn)
    : index(indexIn),
      data(dataIn),
      nonce(0),
      timestamp(time(nullptr))
{
    // Initialize the hash
    hash = calculateHash();
}

Block::Block(uint32_t indexIn, const std::vector<Transaction> &transactionsIn)
    : index(indexIn),
      transactions(transactionsIn),
      nonce(0),
      timestamp(time(nullptr))
{
    // If transactions are given, data can be empty or descriptive
    data = "";
    hash = calculateHash();
}

std::string Block::calculateHash() const
{
    std::stringstream ss;
    ss << index << previousHash << timestamp << data << nonce;

    // Include transaction data in the hash
    for (const auto &tx : transactions)
    {
        ss << tx.calculateHash();
    }

    return Cryptography::sha256(ss.str());
}

void Block::mineBlock(uint32_t difficulty)
{
    // Build a target string of difficulty leading zeros
    std::string target(difficulty, '0');

    while (hash.substr(0, difficulty) != target)
    {
        nonce++;
        hash = calculateHash();
    }
}

bool Block::isValid(uint32_t difficulty) const
{
    // Check hash prefix
    std::string target(difficulty, '0');
    if (hash.substr(0, difficulty) != target)
    {
        return false;
    }

    // Check that the hash hasn't changed
    if (hash != calculateHash())
    {
        return false;
    }

    // Validate each transaction (assuming 'from' is the public key)
    for (const auto &tx : transactions)
    {
        if (!tx.getSignature().empty())
        {
            // If transaction is signed, verify using 'from' as the public key
            if (!tx.verifyTransaction(tx.getFrom()))
            {
                return false;
            }
        }
    }

    return true;
}

std::string Block::serialize() const
{
    json j;
    j["index"] = index;
    j["data"] = data;
    j["hash"] = hash;
    j["previousHash"] = previousHash;
    j["timestamp"] = timestamp;
    j["nonce"] = nonce;

    // Serialize transactions
    json txArray = json::array();
    for (const auto &tx : transactions)
    {
        json txObj;
        txObj["from"] = tx.getFrom();
        txObj["to"] = tx.getTo();
        txObj["amount"] = tx.getAmount();
        txObj["timestamp"] = tx.getTimestamp();
        txObj["signature"] = tx.getSignature();
        txArray.push_back(txObj);
    }
    j["transactions"] = txArray;

    return j.dump();
}

Block Block::deserialize(const std::string &serialized)
{
    json j = json::parse(serialized);

    // Create block from the JSON fields
    Block block(j["index"].get<uint32_t>(), j["data"].get<std::string>());
    block.setPreviousHash(j["previousHash"].get<std::string>());
    block.timestamp = j["timestamp"].get<time_t>();
    block.nonce = j["nonce"].get<uint64_t>();
    block.hash = j["hash"].get<std::string>();

    // Rebuild transactions
    std::vector<Transaction> txs;
    for (auto &txVal : j["transactions"])
    {
        Transaction tx(
            txVal["from"].get<std::string>(),
            txVal["to"].get<std::string>(),
            txVal["amount"].get<double>());
        // In real usage, you'd confirm if the signature is valid or set it.
        const std::string sig = txVal["signature"].get<std::string>();
        if (!sig.empty())
        {
            // We hack around const to set the signature after creation.
            // A better approach is to have a 'setSignature()' method or a specialized constructor.
            const_cast<std::string &>(tx.getSignature()) = sig;
        }
        txs.push_back(tx);
    }
    block.transactions = txs;

    return block;
}
