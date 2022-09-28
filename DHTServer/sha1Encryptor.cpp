#include <iostream>
#include <sstream>
#include <bitset>
#include <openssl/sha.h>
#include <cstring>


unsigned long long SHA1Encrypt(std::string text, int keySpace)
{
    unsigned char hash[SHA_DIGEST_LENGTH];

    char* x = new char[text.length() + 1];
    strcpy(x, text.c_str());

    SHA1((unsigned char*)x, strlen(x), (unsigned char*)&hash);

    char mdString[SHA_DIGEST_LENGTH * 2 + 1];

    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        sprintf(&mdString[i * 2], "%02x", (unsigned int)hash[i]);

    std::string binaryString;

    for (int i = 0; i < sizeof(mdString); ++i)
        binaryString+= std::bitset<8>(mdString[i]).to_string();

    unsigned long long hashKey = std::stoull(binaryString.substr(0, 64), nullptr, 2) % keySpace;

    return hashKey;
};