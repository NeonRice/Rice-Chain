#pragma once

#include <string>
#include <iostream>
#include <bitset>
#include <vector>
#include <sstream>

//IOUTILITY.H
std::string digestToString(std::bitset<32> *digest);

//UTILITY.h
inline std::bitset<32> lrot(std::bitset<32> word, unsigned bit)
{
    return (word << bit) | (word >> (32 - bit));
}

inline std::bitset<32> transform(std::bitset<32> *words)
{
    std::bitset<32> xorTransed = words[0] ^ words[1] ^ words[2] ^ words[3];
    return lrot(xorTransed, 1); //Left rotation by 1 bit
}

void trans1(std::bitset<32> &f, std::bitset<32> &k, std::bitset<32> *vars);

void trans2(std::bitset<32> &f, std::bitset<32> &k, std::bitset<32> *vars);

void trans3(std::bitset<32> &f, std::bitset<32> &k, std::bitset<32> *vars);

void trans4(std::bitset<32> &f, std::bitset<32> &k, std::bitset<32> *vars);

// HASH.h
template <class T, size_t BITSIZE>
std::vector<T> getChunks(const std::string &input)
{
    std::vector<T> chunks;
    for (size_t i = 0; i < input.length(); i += BITSIZE)
        chunks.push_back(T(std::bitset<BITSIZE>(input.substr(i, BITSIZE))));

    return chunks;
}

template <size_t BITSIZE>
struct BitsetObject
{
    std::bitset<BITSIZE> value;

    BitsetObject(std::bitset<BITSIZE> val) : value(val){};
};

template <size_t BITSIZE>
struct Chunk : public BitsetObject<BITSIZE>
{
    Chunk()
    {
        words = getChunks<BitsetObject<BITSIZE / 16>, BITSIZE / 16>(this->value.to_string());
    }
    Chunk(std::bitset<BITSIZE> bitSet) : BitsetObject<BITSIZE>(bitSet)
    {
        words = getChunks<BitsetObject<BITSIZE / 16>, BITSIZE / 16>(this->value.to_string());
    }

    std::vector<BitsetObject<BITSIZE / 16>> words;
};

inline std::string padToCongruent(std::string input)
{
    while (input.size() % 512 != 448)
        input += "0";

    return input;
}

template <class T>
void transformWords(T &chunk, std::bitset<32> *digest)
{
    std::bitset<32> vars[] = {digest[0], digest[1], digest[2],
                              digest[3], digest[4], digest[5], digest[6], digest[7]};

    for (size_t i = 0; i <= 79; ++i) //Looping through words
    {
        std::bitset<32> f, k;

        if (i <= 19)
            trans1(f, k, vars);
        else if (i <= 39)
            trans2(f, k, vars);
        else if (i <= 59)
            trans3(f, k, vars);
        else if (i <= 79)
            trans4(f, k, vars);

        std::bitset<32> temp = (lrot(vars[0], 5).to_ullong() + f.to_ullong() + vars[4].to_ullong() + k.to_ullong() + chunk.words[i].value.to_ullong());

        vars[5] = lrot(vars[4], 10) ^ vars[3];
        vars[7] = vars[4].to_ullong() + vars[6].to_ullong() + lrot(f.to_ullong() + k.to_ullong(), 10).to_ullong();
        vars[4] = vars[3];
        vars[3] = vars[2];
        vars[2] = lrot(vars[1], 30);
        vars[6] = vars[5] | vars[1] ^ vars[2];
        vars[1] = vars[0];
        vars[0] = temp;
    }

    digest[0] = std::bitset<32>(digest[0].to_ullong() + vars[0].to_ullong());
    digest[1] = std::bitset<32>(digest[1].to_ullong() + vars[1].to_ullong());
    digest[2] = std::bitset<32>(digest[2].to_ullong() + vars[2].to_ullong());
    digest[3] = std::bitset<32>(digest[3].to_ullong() + vars[3].to_ullong());
    digest[4] = std::bitset<32>(digest[4].to_ullong() + vars[4].to_ullong());
    digest[5] = std::bitset<32>(digest[5].to_ullong() + vars[5].to_ullong());
    digest[6] = std::bitset<32>(digest[6].to_ullong() + vars[6].to_ullong());
    digest[7] = std::bitset<32>(digest[7].to_ullong() + vars[7].to_ullong());
}

template <class T>
inline void extendWords(T &words)
{
    int i = 16;
    for (size_t i = 16; i <= 79; i++)
    {
        std::bitset<32> wordArr[4] = {words[i - 3].value, words[i - 8].value,
                                      words[i - 14].value, words[i - 16].value};
        words.push_back(transform(wordArr));
    }
}

std::string hash(const std::string &input);
