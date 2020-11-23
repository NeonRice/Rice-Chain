#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <random>
#include <future>
#include <unordered_map>

#include "Block.h"
#include "Transaction.h"
#include "TransactionHelper.h"
#include "VectorHelper.h"

struct Miner
{
    static constexpr unsigned THREADS = 12;
    static constexpr unsigned TRANSACTIONS_PER_BLOCK = 100;
    static constexpr unsigned NEEDED_TRANSACTIONS = THREADS * TRANSACTIONS_PER_BLOCK;
    unsigned COMPLEXITY = 3;

    std::unordered_map<std::thread::id, int> triesPerThread;
    std::unordered_map<std::thread::id, int> successfulTriesPerThread;

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
        const std::string prevBlockHash = lastBlockHash;
        const std::string merkelRootHash = this->merkleRoot(getTransactionIds(transactions));

        Block block = Block(getNonce(), prevBlockHash, COMPLEXITY, merkelRootHash, transactions);
        std::string blockHash = hash(block);

        while (!isDone)
        {
            triesPerThread[std::this_thread::get_id()] += 1;
            if (isAdequateComplexity(blockHash) && !isDone)
            {
                isDone.store(true);
                successfulTriesPerThread[std::this_thread::get_id()] += 1;
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

        std::vector<std::vector<Transaction>> splitTransactions =
            SplitVector<Transaction>(transactions, transactions.size() / TRANSACTIONS_PER_BLOCK);

        for (auto &&transaction : splitTransactions)
            threads.push_back(std::async(&Miner::tryBlock, this, transaction));

        for (size_t i = splitTransactions.size(); i < THREADS; i++)
            threads.push_back(std::async(&Miner::tryBlock, this, splitTransactions[0]));

        std::vector<Block> minedBlocks;
        for (auto &&thread : threads)
            minedBlocks.push_back(thread.get());

        for (size_t i = 0; i < minedBlocks.size(); i++)
        {
            std::string minedBlockHash = hash(minedBlocks[i]);
            if (isAdequateComplexity(minedBlockHash))
            {
                isDone.store(false);
                lastBlockHash = minedBlockHash;
                transactions.erase(
                    transactions.begin() + i * minedBlocks[i].transactions.size(),
                    transactions.begin() + i * minedBlocks[i].transactions.size() +
                        minedBlocks[i].transactions.size());
                return minedBlocks[i];
            }
        }
    }
};