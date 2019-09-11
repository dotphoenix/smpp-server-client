#ifndef BASE64_H
#define BASE64_H
#include <string>
#include <vector>
std::string base64_encode(unsigned char const* , unsigned int len, bool using_standard = false);
std::vector<unsigned char> base64_decode(std::string const& s, bool using_standard = false);

#endif