#pragma once

#include <vector>
#include "Block.h"
#include "Transaction.h"

/*!
 * \brief Manages the chain (vector of blocks), difficulty, and pending transactions.
 */
class Blockchain
{
public:
    Blockchain();

    // Add a block containing only textual data (no transactions)
    void addBlock(const std::string &data);

    // Add a transaction to the list of pending transactions
    void addTransaction(const Transaction &transaction);

    // Mine a new block containing all pending transactions
    void minePendingTransactions();

    // Validate entire chain
    bool isChainValid() const;

    // File I/O
    void exportToFile(const std::string &filename) const;
    bool importFromFile(const std::string &filename);

    // Getters
    const std::vector<Block> &getChain() const { return chain; }
    uint32_t getDifficulty() const { return difficulty; }
    const std::vector<Transaction> &getPendingTransactions() const { return pendingTransactions; }

private:
    std::vector<Block> chain;
    uint32_t difficulty;
    std::vector<Transaction> pendingTransactions;

    Block createGenesisBlock() const;
};
