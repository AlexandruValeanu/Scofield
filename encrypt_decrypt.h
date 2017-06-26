#ifndef SCOFIELD_ENCRYPT_DECRYPT_H
#define SCOFIELD_ENCRYPT_DECRYPT_H

#include <string>

void encrypt(const std::string &key, std::string &text);
void decrypt(const std::string &key, std::string &text);

#endif //SCOFIELD_ENCRYPT_DECRYPT_H
