#pragma once

#include <string>

#include "User.h"
#include "../external/hash.h"

class Transaction
{
    std::string sender;
    std::string receiver;
    unsigned amount;

    public:
        std::string transaction_id;

        Transaction(const std::string &sender, const std::string &receiver, const unsigned amount) :
            sender(sender), receiver(receiver), amount(amount)
            { transaction_id = hash(sender + receiver + std::to_string(amount)); }

        std::string getSender()
        {
            return sender;
        }

        std::string getReceiver()
        {
            return receiver;
        }

        unsigned long long getAmount()
        {
            return amount;
        }

};