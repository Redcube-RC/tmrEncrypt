
#ifndef AES_H
#define AES_H

#include <string>

std::string aes(std::string& plain_text, std::string& key);
std::string in_aes(std::string& text, std::string& key);
std::string int_to_chs(long long num);
int ch_to_int(char& ch);
long long str_long(std::string str);

#endif // AES_H
