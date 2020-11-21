#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <random>
#include <future>

#include "Block.h"
#include "Transaction.h"
#include "TransactionHelper.h"
#include "VectorHelper.h"

constexpr unsigned THREADS = 12;
unsigned COMPLEXITY = 4;

struct Miner
{
    //unsigned triesPerThread[THREADS] = { 0 };
    //unsigned successfulTriesPerThread[THREADS] = { 0 };
    unsigned tries = 0;
    unsigned successfulTries = 0;

    std::string lastBlockHash = hash("default");
    std::atomic<bool> isDone;

    Miner() { isDone.store(false); }

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

    unsigned getNonce()
    {
        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_int_distribution<unsigned> distr(0, std::numeric_limits<unsigned>::max());

        return distr(generator);
    }

    inline bool isAdequateComplexity(const std::string &hash)
    {
        return hash.substr(0, COMPLEXITY) == std::string(COMPLEXITY, '0');
    }

    Block tryBlock(std::vector<Transaction> transactions)
    {
        const unsigned nonce = getNonce();
        const std::string prevBlockHash = lastBlockHash;
        const std::string merkelRootHash = this->merkleRoot(getTransactionIds(transactions));

        Block block = Block(nonce, prevBlockHash, COMPLEXITY, merkelRootHash, transactions);
        std::string blockHash = hash(block);

        while (!isDone)
        {
            if (isAdequateComplexity(blockHash))
            {
                isDone.store(true);
                break;
            }
            block = Block(getNonce(), prevBlockHash, COMPLEXITY, merkelRootHash, transactions);
            blockHash = hash(block);
        }
        return block;
    }

    Block mineBlock(std::vector<Transaction> &transactions)
    {
        std::vector<std::future<Block>> threads;
        std::vector<std::vector<Transaction>> splitTransactions = SplitVector<Transaction>(transactions, THREADS);

        for (size_t i = 0; i < THREADS; i++)
            threads.push_back(std::async(&Miner::tryBlock, this, std::ref(splitTransactions[i])));

        for (auto &&block : threads)
        {
            Block minedBlock = block.get();
            std::string minedBlockHash = hash(minedBlock);
            if (isAdequateComplexity(minedBlockHash))
            {
                isDone.store(false);
                lastBlockHash = minedBlockHash;
                return minedBlock;
            }
        }
        //
    }
};