#include "hash.h"
#include <iomanip>

//IOUTILITY.H

std::string digestToString(std::bitset<32> *digest)
{
    std::stringstream ss;
    ss << std::setw(8) << std::setfill('0') << std::hex << digest[0].to_ullong()
       << std::setw(8) << digest[1].to_ullong() << std::setw(8) << digest[2].to_ullong()
       << std::setw(8) << digest[3].to_ullong() << std::setw(8) << digest[4].to_ullong()
       << std::setw(8) << digest[5].to_ullong() << std::setw(8) << digest[6].to_ullong()
       << std::setw(8) << digest[7].to_ullong();
    return ss.str();
}

void trans1(std::bitset<32> &f, std::bitset<32> &k, std::bitset<32> *vars)
{
    f = ((vars[1] & vars[2]) | (~vars[1] & vars[3]));
    k = std::bitset<32>(std::string("01011010100000100111100110011001"));
}

void trans2(std::bitset<32> &f, std::bitset<32> &k, std::bitset<32> *vars)
{
    f = vars[1] ^ vars[2] ^ vars[3];
    k = std::bitset<32>(std::string("01101110110110011110101110100001"));
}

void trans3(std::bitset<32> &f, std::bitset<32> &k, std::bitset<32> *vars)
{
    f = (vars[1] & vars[2]) | (vars[1] & vars[3]) | (vars[2] & vars[3]);
    k = std::bitset<32>(std::string("10001111000110111011110011011100"));
}

void trans4(std::bitset<32> &f, std::bitset<32> &k, std::bitset<32> *vars)
{
    f = vars[1] ^ vars[2] ^ vars[3];
    k = std::bitset<32>(std::string("11001010011000101100000111010110"));
}

std::string hash(const std::string &input)
{
    std::string binarifiedString = "";

    for (auto it = input.begin(); it != input.end(); ++it)
    {
        //Converting char to ASCII and then converting it to binary
        int charCode = *it;
        std::bitset<8> binaryNum(charCode);
        binarifiedString += binaryNum.to_string(); //Concat to a big chunk of a string
    }
    binarifiedString += "1";

    std::string paddedString = padToCongruent(binarifiedString);

    paddedString += std::bitset<64>(binarifiedString.size() - 1).to_string();

    std::vector<Chunk<512>> chunks = getChunks<Chunk<512>, 512>(paddedString); //Processes words aswell as chunks?

    std::bitset<32> h0 = std::bitset<32>(std::string("01100111010001010010001100000001"));
    std::bitset<32> h1 = std::bitset<32>(std::string("11101111110011011010101110001001"));
    std::bitset<32> h2 = std::bitset<32>(std::string("10011000101110101101110011111110"));
    std::bitset<32> h3 = std::bitset<32>(std::string("00010000001100100101010001110110"));
    std::bitset<32> h4 = std::bitset<32>(std::string("11000011110100101110000111110000"));
    std::bitset<32> h5 = std::bitset<32>(std::string("00101110100101011010111010001001"));
    std::bitset<32> h6 = std::bitset<32>(std::string("11111000001000000101100010001010"));
    std::bitset<32> h7 = std::bitset<32>(std::string("00000010001000100101010000110010"));

    std::bitset<32> digest[] = {h0, h1, h2, h3, h4, h5, h6, h7};

    // Proccess each chunk seperately
    for (auto &&chunk : chunks)
    {
        extendWords(chunk.words);
        transformWords(chunk, digest);
    }

    return digestToString(digest);
}
