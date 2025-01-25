#include <gtest/gtest.h>
#include "Transaction.h"
#include "Cryptography.h"

TEST(TransactionTests, Creation)
{
    Transaction tx("Alice", "Bob", 100.0);
    EXPECT_EQ(tx.getFrom(), "Alice");
    EXPECT_EQ(tx.getTo(), "Bob");
    EXPECT_DOUBLE_EQ(tx.getAmount(), 100.0);
}

TEST(TransactionTests, SigningAndVerification)
{
    // We'll treat 'AlicePublicKey' as the one that matches the private key generated below
    auto keys = Cryptography::generateKeyPair();
    std::string privateKey = keys.first;
    std::string publicKey = keys.second;

    Transaction tx("AlicePublicKey", "BobPublicKey", 50.0);
    tx.signTransaction(privateKey);

    // Check that verification passes using "from" as "AlicePublicKey"
    EXPECT_TRUE(tx.verifyTransaction(publicKey));
}
