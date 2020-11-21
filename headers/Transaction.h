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
        bool isValid = true;
        bool isComplete = false;

        Transaction(const std::string &sender, const std::string &receiver, const unsigned amount) :
            sender(sender), receiver(receiver), amount(amount)
            { transaction_id = hash(sender + receiver + std::to_string(amount)); } // @TODO refactor

        std::string getSender()
        {
            return sender;
        }

        std::string getReceiver()
        {
            return receiver;
        }

        unsigned getAmount()
        {
            return amount;
        }

        operator std::string() const
        {
            return sender + receiver + std::to_string(amount);
        }

};