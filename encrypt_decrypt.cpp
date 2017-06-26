#include <iostream>
#include "encrypt_decrypt.h"

void encrypt(const std::string &key, std::string &text){
    std::string enc_text;
    size_t p = 0;

    for (auto &c: text){
        c ^= key[p];

        if (++p == key.size())
            p = 0;
    }
}

void decrypt(const std::string &key, std::string &text){
    encrypt(key, text);
}
