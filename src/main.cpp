#include <iostream>
#include <chrono>

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
    std::vector<Transaction> pickedTransactions =
        pickRandTransactions(miner.NEEDED_TRANSACTIONS, transactionPool, userPool);

    auto start = std::chrono::high_resolution_clock::now();
    while (!pickedTransactions.empty())
    {
        if (pickedTransactions.size() != miner.NEEDED_TRANSACTIONS)
        {
            std::vector<Transaction> additional = pickRandTransactions(
                miner.NEEDED_TRANSACTIONS - pickedTransactions.size(), transactionPool, userPool);

            pickedTransactions.insert(pickedTransactions.end(), additional.begin(), additional.end());
        }

        auto start = std::chrono::high_resolution_clock::now();
        Block minedBlock = miner.mineBlock(pickedTransactions);
        auto end = std::chrono::high_resolution_clock::now();
        
        executeTransactionPool(validateTransactions(minedBlock.transactions, userPool), userPool);
        std::cout << "Block mined! " << hash(minedBlock) << "\n Time Elapsed: "
                  << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
                  << "s" << std::endl; 
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Mining took " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
              << " s" << std::endl;

    for (auto &&threadInfo : miner.triesPerThread)
    {
        std::cout << threadInfo.first << " tries " << threadInfo.second << std::endl;
    }

    for (auto &&threadInfo : miner.successfulTriesPerThread)
    {
        std::cout << threadInfo.first << " successful tries " << threadInfo.second << std::endl;
    }
    

    return 0;
}