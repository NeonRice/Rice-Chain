#include <random>
#include <iostream>

#include "../external/hash.h"
#include "../headers/Generators.h"
#include "../headers/Transaction.h"
#include "../headers/User.h"

std::vector<Transaction*> pickRandTransactions(const unsigned &amount, std::vector<Transaction> &transactions)
{
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distr(0, transactions.size());

    std::vector<Transaction*> pickedTransactions;

    for (size_t i = 0; i < amount; i++)
    {
        int transactionNr = distr(generator);
        pickedTransactions.push_back(&transactions[transactionNr]);
    }

    return pickedTransactions;
} 

int main()
{
    std::cout << "Generating users..." << std::endl;
    std::vector<User> userPool = generateUsers(1000);
    std::cout << "Generating transactions..." << std::endl;
    std::vector<Transaction> transactionPool = generateTransactions(10000, userPool);
    std::vector<Transaction*> pickedTransactions = pickRandTransactions(100, transactionPool);
    std::cout << pickedTransactions.size() << std::endl;    

    return 0;
}