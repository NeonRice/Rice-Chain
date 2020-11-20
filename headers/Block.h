#pragma once

#include <string>
#include <ctime>
#include <vector>

#include "Transaction.h"
#include "TransactionHelper.h"

class Block
{
    public:
        std::string prevBlockHash;
        std::string merkelRootHash;
        std::string version = "0.2";
        std::string difficultyTarget;
        unsigned long nonce;
        std::time_t timestamp;
        std::vector<Transaction> transactions;

        operator std::string() const
        {
            return prevBlockHash + merkelRootHash + version
                + difficultyTarget + std::to_string(nonce) 
                + std::to_string(timestamp) + getStringFromTransactions(transactions);
        }

        Block* changeDifficultyTarget(unsigned complexity)
        {
            this->difficultyTarget = std::string("0", complexity);

            return this;
        }

        Block(
            const unsigned long &nonce,
            const std::string prevBlockHash, 
            const unsigned &complexity, 
            const std::string &rootHash,
            std::vector<Transaction> &transactions
            ) : nonce(nonce), prevBlockHash(prevBlockHash),
                transactions(transactions), merkelRootHash(rootHash)
            {
                timestamp = std::time(nullptr);
                changeDifficultyTarget(complexity);
            };
        
        Block() {}
        Block(const Block& b) : prevBlockHash(b.prevBlockHash), merkelRootHash(b.merkelRootHash), version(b.version), difficultyTarget(b.difficultyTarget), nonce(b.nonce), timestamp(b.timestamp), transactions(b.transactions) {};

};