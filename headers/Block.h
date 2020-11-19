#pragma once

#include <string>
#include <ctime>
#include <vector>

#include "Transaction.h"

class Block
{
    public:
        std::string prevBlockHash;
        std::string merkelRootHash;
        std::string version;
        std::string difficultyTarget;
        unsigned long long nonce;
        std::time_t timestamp;
        std::vector<Transaction> transactions;


};