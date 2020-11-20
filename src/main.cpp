#include <iostream>

#include "../external/hash.h"
#include "../headers/Generators.h"
#include "../headers/Transaction.h"
#include "../headers/User.h"
#include "../headers/UserHelper.h"
#include "../headers/TransactionHelper.h"
#include "../headers/Miner.h"

int main()
{
    std::cout << "Generating users..." << std::endl;
    std::vector<User> userPool = generateUsers(1000);
    std::cout << "Generating transactions..." << std::endl;
    std::vector<Transaction> transactionPool = generateTransactions(10000, userPool);
    std::vector<Transaction> pickedTransactions = pickRandTransactions(500, transactionPool, userPool);
    Miner miner;
    Block b = miner.mineBlock(pickedTransactions);
    std::cout << "Block mined! " << hash(b) << std::endl;
    

    return 0;
}