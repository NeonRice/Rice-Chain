#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <random>
#include <future>

#include "Block.h"
#include "Transaction.h"
#include "TransactionHelper.h"

constexpr unsigned THREADS = 5;
unsigned COMPLEXITY = 3;


struct Miner
{
    //unsigned triesPerThread[THREADS] = { 0 };
    //unsigned successfulTriesPerThread[THREADS] = { 0 };
    unsigned tries = 0;
    unsigned successfulTries = 0;

    std::string lastBlockHash = hash("default");
    std::atomic<bool> isDone = false;
    //std::atomic<Block> minedBlock;

    std::string merkleRoot(std::vector<std::string> transactionIds)
    {
        std::vector<std::string> temp = transactionIds;
        std::vector<std::string> temp2;
        while (temp.size() > 1)
        {
            if (temp.size() & 1) // checks if size is even, if its not, adds the same element to the back
                temp.push_back(temp.back());

            for (int i = 0; i < temp.size(); i += 2)          // loops through the vector of hashes of previuos layer
                temp2.push_back(hash(temp[i] + temp[i + 1])); // push back hash of two nearby elements

            temp = temp2;
            temp2.clear();
        }
        return temp[0];
    }

    unsigned long getNonce()
    {
        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_int_distribution<unsigned long> distr(0, std::numeric_limits<unsigned long>::max());

        return distr(generator);
    }

    inline bool isAdequateComplexity(const std::string &hash)
    {
        return hash.substr(0, COMPLEXITY) == std::string("0", COMPLEXITY);
    }

    void tryBlock(std::vector<Transaction> transactions)
    {
        const unsigned long nonce = getNonce();
        const std::string prevBlockHash = lastBlockHash;
        const std::string merkelRootHash = this->merkleRoot(getTransactionIds(transactions));
        
        Block block = Block(nonce, prevBlockHash, COMPLEXITY, merkelRootHash, transactions);
        std::string blockHash = hash(block);

        while (!isDone)
        {
            if (isAdequateComplexity(blockHash))
            {
                isDone.store(true);
                //minedBlock.store(block);
                break;
            }
            block = Block(nonce, prevBlockHash, COMPLEXITY, merkelRootHash, transactions);
            blockHash = hash(block);
        }
    }

    Block mineBlock(std::vector<Transaction> transactions)
    {
        std::vector<std::thread> threads;

        for (size_t i = 0; i < THREADS; i++)
        {
            // Danger here
            threads.push_back(std::thread(&Miner::tryBlock, std::vector<Transaction>(
                transactions.begin() + i * 100, transactions.begin() + i * 100 + 100
                )
            ));
            /* std::vector<Transaction> transactions(
                            transactions.begin() + i * 100, transactions.begin() + i * 100 + 100
                            );
            //threads.push_back(std::async(&tryBlock, std::ref(transactions)));
            std::async(&tryBlock, std::ref(transactions)); */
        }

            for (auto &&thread : threads)
                thread.join();
            
            //return minedBlock.load();
            /* for (auto &&block : threads)
            {
                if (isAdequateComplexity(hash(block.get())))
                    return block.get();
            } */
    }
};