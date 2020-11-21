#pragma once

#include "User.h"
#include <string>
#include <algorithm>

inline User* getUserByKey(const std::string &pKey, std::vector<User> &userPool)
{
    auto userPtr = std::find_if(std::begin(userPool), std::end(userPool), 
    [&] (User const& user) { return user.public_key == pKey; });

    if (userPtr != userPool.end())
        return &(*userPtr);
    return nullptr;
}