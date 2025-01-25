#include "Transaction.h"
#include "Cryptography.h"
#include <sstream>

Transaction::Transaction(const std::string &fromIn, const std::string &toIn, double amountIn)
    : from(fromIn),
      to(toIn),
      amount(amountIn),
      timestamp(time(nullptr))
{
}

void Transaction::signTransaction(const std::string &privateKey)
{
    // Sign the transaction hash
    signature = Cryptography::sign(calculateHash(), privateKey);
}

bool Transaction::verifyTransaction(const std::string &publicKey) const
{
    // If there's no signature, consider it invalid
    if (signature.empty())
    {
        return false;
    }
    // Verify the signature against the transaction hash
    return Cryptography::verify(calculateHash(), signature, publicKey);
}

std::string Transaction::calculateHash() const
{
    std::stringstream ss;
    ss << from << to << amount << timestamp;
    return Cryptography::sha256(ss.str());
}
