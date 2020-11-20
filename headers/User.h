#pragma once

#include <string>
#include "../external/hash.h"

class User
{
    std::string name;
    unsigned balance;

    public:
        std::string public_key;

        User(const std::string &name, const unsigned &balance) : name(name), balance(balance) {
            public_key = hash(name);
        };
    
    User setName(const std::string &name)
    {
        this->name = name;

        return *this;
    }

    User setBalance(const unsigned &balance)
    {
        this->balance = balance;

        return *this;
    }

    std::string getName()
    {
        return name;
    }

    unsigned getBalance()
    {
        return balance;
    }
};