#include "Blockchain.h"
#include "Exceptions.h"
#include <fstream>
#include <json.hpp>

using json = nlohmann::json;

Blockchain::Blockchain()
    : difficulty(4)
{
    // Create the genesis block
    chain.push_back(createGenesisBlock());
}

Block Blockchain::createGenesisBlock() const
{
    // A simple genesis block
    return Block(0, "Genesis Block");
}

void Blockchain::addBlock(const std::string &data)
{
    // Create a new block containing just textual data
    Block newBlock(chain.size(), data);
    newBlock.setPreviousHash(chain.back().getHash());
    newBlock.mineBlock(difficulty);

    chain.push_back(newBlock);
}

void Blockchain::addTransaction(const Transaction &transaction)
{
    // Store transaction in pending transactions to be mined
    pendingTransactions.push_back(transaction);
}

void Blockchain::minePendingTransactions()
{
    if (pendingTransactions.empty())
    {
        // Nothing to mine
        return;
    }

    // Create a block with all pending transactions
    Block newBlock(chain.size(), pendingTransactions);
    newBlock.setPreviousHash(chain.back().getHash());

    // Mine this block
    newBlock.mineBlock(difficulty);

    // Add it to the chain
    chain.push_back(newBlock);

    // Clear pending transactions
    pendingTransactions.clear();
}

bool Blockchain::isChainValid() const
{
    // Iterate from the second block to the end
    for (size_t i = 1; i < chain.size(); ++i)
    {
        const Block &currentBlock = chain[i];
        const Block &previousBlock = chain[i - 1];

        // Check continuity
        if (currentBlock.getPreviousHash() != previousBlock.getHash())
        {
            return false;
        }

        // Check current block's validity
        if (!currentBlock.isValid(difficulty))
        {
            return false;
        }
    }
    return true;
}

void Blockchain::exportToFile(const std::string &filename) const
{
    // Export entire chain as a JSON array
    json chainArray = json::array();
    for (const auto &block : chain)
    {
        // block.serialize() is already JSON (as a string), let's parse it so we can build an array
        std::string blockStr = block.serialize();
        json blockJson = json::parse(blockStr);
        chainArray.push_back(blockJson);
    }

    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw FileOperationException("Failed to open file for writing: " + filename);
    }

    file << chainArray.dump(4); // 4 = pretty print indent
    file.close();
}

bool Blockchain::importFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw FileOperationException("Failed to open file for reading: " + filename);
    }

    json chainArray;
    try
    {
        file >> chainArray;
    }
    catch (...)
    {
        throw FileOperationException("Invalid JSON format in file: " + filename);
    }

    if (!chainArray.is_array())
    {
        return false;
    }

    std::vector<Block> newChain;
    for (auto &blockJson : chainArray)
    {
        std::string blockStr = blockJson.dump();
        Block blk = Block::deserialize(blockStr);
        newChain.push_back(blk);
    }
    file.close();

    // Test if new chain is valid
    auto oldChain = chain;
    chain = newChain;
    if (!isChainValid())
    {
        // Revert
        chain = oldChain;
        return false;
    }

    // If valid, keep new chain
    return true;
}
