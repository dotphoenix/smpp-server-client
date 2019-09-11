#ifndef __DATA_ENCODING_H__
#define __DATA_ENCODING_H__

#include <vector>
//void linux_split_utf8_string(char *fulline);
void windows_split_utf8_string(const std::string& s, std::vector<std::string>& strings);

//比如短信内容是:欢迎光临阿尔卑斯山，号码是18665152605，则内容转换为：亲爱的2605用户，欢迎光临阿尔卑斯山
std::string append_header_to_string(const std::string& content, const std::string& number);

//比如短信内容是:欢迎光临阿尔卑斯山
//index为0的内容转换为：欢.迎光临阿尔卑斯山
//index为0的内容转换为：欢迎.光临阿尔卑斯山
//index为10的内容转换为：欢,迎光临阿尔卑斯山
std::string insert_char_to_string(const std::string& content, int index);

#endif