#pragma once

#include "User.h"
#include "Transaction.h"

#include <ctime>
#include <vector>

const unsigned MAX_USER_BALANCE = 1000000;
const unsigned MIN_USER_BALANCE = 100;
std::time_t generatorSeed = std::time(nullptr);

inline User generateUser(std::string namePostfix)
{
    srand(generatorSeed++);

    return User("User" + namePostfix, rand() % (MAX_USER_BALANCE-MIN_USER_BALANCE) + MIN_USER_BALANCE);
}

inline std::vector<User> generateUsers(const unsigned &amount, const unsigned &postfix = 0)
{
    unsigned userPostfix = postfix;
    std::vector<User> users;

    for (size_t i = 0; i < amount; i++)
    {
        userPostfix += i;
        users.push_back(generateUser(std::to_string(userPostfix)));
    }

    return users;
}

inline Transaction generateTransaction(std::vector<User> userPool)
{
    srand(generatorSeed++);
    std::string user1, user2;

    while (user1 == user2)
    {
        user1 = userPool[rand() % userPool.size()].public_key;
        user2 = userPool[rand() % userPool.size()].public_key;
    }

    return Transaction(user1, user2, rand() % (MAX_USER_BALANCE-MIN_USER_BALANCE) + MIN_USER_BALANCE);
}

inline std::vector<Transaction> generateTransactions(const unsigned &amount, std::vector<User> userPool)
{
    std::vector<Transaction> transactions;

    for (size_t i = 0; i < amount; i++)
    {
        transactions.push_back(generateTransaction(userPool));
    }
    
    return transactions;
}   