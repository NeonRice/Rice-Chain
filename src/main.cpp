#include <iostream>

#include "../external/hash.h"
#include "../headers/Generators.h"
#include "../headers/Transaction.h"
#include "../headers/User.h"

int main()
{
    std::cout << "Generating users..." << std::endl;
    std::vector<User> userPool = generateUsers(1000);
    std::cout << "Generating transactions..." << std::endl;
    std::vector<Transaction> transactionPool = generateTransactions(10000, userPool);
    
    for (auto &&transaction : transactionPool)
    {
        std::cout << transaction.getSender() << " " << transaction.getReceiver() << std::endl;
        std::cout << transaction.transaction_id << std::endl;
    }
    

    return 0;
}