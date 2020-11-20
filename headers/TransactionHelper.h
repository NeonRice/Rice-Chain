#pragma once

#include <vector>
#include <random>

#include "Transaction.h"
#include "User.h"
#include "UserHelper.h"

// Not good in an actual application, as you can send money to an invalid user.
// Also no check for nullptr, but for this example it will satisfy.
inline bool isValidTransaction(Transaction transaction, std::vector<User> &userPool)
{
     if (transaction.transaction_id == hash(
        transaction.getSender() + transaction.getReceiver() + std::to_string(transaction.getAmount())
        ) && transaction.getAmount() <= getUserByKey(transaction.getSender(), userPool)->getBalance())
        return true;
    return false;
}

Transaction executeTransaction(Transaction toExecute, std::vector<User> &userPool)
{
    // Must be verified before execution   
    User *sender = getUserByKey(toExecute.getSender(), userPool);
    User *receiver = getUserByKey(toExecute.getReceiver(), userPool);

    sender->setBalance(sender->getBalance() - toExecute.getAmount());
    receiver->setBalance(receiver->getBalance() + toExecute.getAmount());

    return toExecute;
}

std::vector<Transaction> pickRandTransactions(
    const unsigned &amount,
    std::vector<Transaction> &transactions,
    std::vector<User> &userPool )
{
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distr(0, transactions.size());

    std::vector<Transaction> pickedTransactions;

    for (size_t i = 0; i < amount; i++)
    {
        int transactionNr = distr(generator);
        Transaction transaction = transactions[transactionNr];
        if (isValidTransaction(transaction, userPool))
        {
            executeTransaction(transaction, userPool);
            pickedTransactions.push_back(transaction);
            transactions.erase(transactions.begin() + transactionNr);
        }
    }
    return pickedTransactions;
} 

std::vector<std::string> getTransactionIds(std::vector<Transaction> &transactions)
{
    std::vector<std::string> transactionIds;
    for (auto &&transaction : transactions)
        transactionIds.push_back(transaction.transaction_id);
    return transactionIds;
}

std::string getStringFromTransactions(std::vector<Transaction> transactions)
{
    std::string result = "";
    for (auto &&transaction : transactions)
        result += transaction.transaction_id;
    return result;
}