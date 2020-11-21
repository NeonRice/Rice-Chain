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
    std::cout << "Mining..." << std::endl;
    Miner miner;
    std::vector<Transaction> pickedTransactions;
    while (transactionPool.size() != 0)
    {
        pickedTransactions = pickRandTransactions(500, transactionPool, userPool);
        Block minedBlock = miner.mineBlock(pickedTransactions);
        executeTransactionPool(validateTransactions(minedBlock.transactions, userPool), userPool);
        std::cout << "Block mined! " << hash(minedBlock) << std::endl;
    }
    
    
    
    

    return 0;
}