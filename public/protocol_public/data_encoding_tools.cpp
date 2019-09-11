#include "data_encoding_tools.hpp"
#include "algorithm.hpp"
/*
无论你是怎么搜索，百度出来匹配中文的正则基本都是上述的表达式。因此linux环境下3字节的UTF-8也用这个正则来匹配显然是不正确的。
Linux环境下的正则匹配规则中

\un，其中的 n?是一个用四个十六进制数字表示的 Unicode字符。并不支持三个字节的utf-8

\xn，其中?n?为十六进制转义值。十六进制转义值必须为确定的两个数字长。例如， '\x41'匹配 "A"。'\x041' 则等价于 '\x04' & "1"。正则表达式中可以使用 ASCII编码。

因此得考虑将三个字节转变成2个字节，再进行中文的匹配。

先看双字节的unicode是如何转化成三字节的utf-8

|  Unicode符号范围      |  UTF-8编码方式
n |  (十六进制)           | (二进制)
---+-----------------------+------------------------------------------------------
1 | 0000 0000 - 0000 007F |                                              0xxxxxxx
2 | 0000 0080 - 0000 07FF |                                     110xxxxx 10xxxxxx
3 | 0000 0800 - 0000 FFFF |                            1110xxxx 10xxxxxx 10xxxxxx
4 | 0001 0000 - 0010 FFFF |                   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
5 | 0020 0000 - 03FF FFFF |          111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
6 | 0400 0000 - 7FFF FFFF | 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
查表可知，

\u4e00-\u9fa5
处于n=3的编码处，例如4E00，那么将它先转化成16进制(不足16位的在前面补0)

0000 1110 0000 0000
根据第三行的格式，也就是将上述16个比特位重新断句
0000 111000 000000
从左至右分别放置到以下的xx处
1110xxxx 10xxxxxx 10xxxxxx
得到三字节的UTF-8编码
11100000 10111000 10000000
也就是0xE0 B8 80
经计算可知，无法只用一个范围来确定三字节的中文要怎么表示，所以还是将其转化成双字节来匹配中文。

将上述推理反推，就能知道三字节的UTF-8如何转变成双字节的unicode编码。既然知道原理，就没有必要再用正则来判断字符是否为中文了。



参考:
https://stackoverflow.com/questions/40054732/c-iterate-utf-8-string-with-mixed-length-of-characters

*/

#if 0
void linux_split_utf8_string(char *fulline) 
{
	int sum = strlen(fulline);
	std::string lines;
	for(int cur = 0;cur<sum;cur++)
	{		
		char t = fulline[cur];
		if((t&0xE0) == 0xE0)
		{	//3byte
			lines = lines + t + fulline[cur+1] + fulline[cur+2];
			cur += 3;

		}
		else if((t&0xC0) == 0xC0)
		{//2byte
			lines = lines + t + fulline[cur+1];
			cur += 2;
		}
		else
		{//1byte
			lines = lines + t;
			cur++;
		}
		OutputDebugString(s2ws(lines).c_str());
	}
}

#endif

void windows_split_utf8_string(const std::string& s, std::vector<std::string>& strings)
{
	for (size_t i = 0; i < s.length();)
	{
		int cplen = 1;
		if ((s[i] & 0xf8) == 0xf0)      // 11111000, 11110000
		{
			cplen = 4;
		}
		else if ((s[i] & 0xf0) == 0xe0) // 11100000
		{
			cplen = 3;
		}
		else if ((s[i] & 0xe0) == 0xc0) // 11000000
		{
			cplen = 2;
		}
		if ((i + cplen) > s.length())
		{
			cplen = 1;
		}
		strings.push_back(s.substr(i, cplen));
		i += cplen;
	}
}

std::string append_header_to_string(const std::string& content, const std::string& number)
{
	size_t n = 4; //取最后n个数字
#ifdef WIN32
	const std::wstring wh1 = L"尾号为";
	const std::wstring wh2 = L"的客户:";
	std::string h1 = ws2s(wh1);
	std::string h2 = ws2s(wh2);
#else
	const std::string h1 = "尾号";
	const std::string h2 = "客户:";
#endif
	std::string h3;
	if(number.length() <= n) 
	{
		h3 = number;
	}
	else
	{
		h3 = number.substr(number.length() - 4, 4);
	}
	std::string s = h1 + h3 + h2 + content;
	return s;
}


std::string insert_char_to_string(const std::string& content , int index)
{
	const int CHAR_COUNT = 80;
	const char chars[CHAR_COUNT] = {',', '.', ';', '#', '$', '^', '*', ' ', '~', '(', ')', '[', ']', '&', ':', '<', '>', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8'
	, '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F'
	, 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
	std::vector<std::string> content_vec;
	windows_split_utf8_string(content, content_vec);
	int len = 0; //只在中文后面插入，单字节的字符(英文，数字等)不插入
	for(int i = 0; i < content_vec.size(); i++)
	{
		if(content_vec.at(i).length() > 1)
		{
			len++;
		}
	}
	//转了第几圈了
	int round = index / len;
	int pos = index % len;
	char c = chars[round % CHAR_COUNT];
	std::string new_content;

	int total_length = content_vec.size();
	bool already_insert = false;
	for(size_t i = 0; i < total_length; i++)
	{
		new_content += content_vec.at(i);
		if(i >= pos && content_vec.at(i).length() > 1 && !already_insert) //单字节后面不插入
		{
			new_content += c;
			already_insert = true;
		}
	}
	return new_content;
}

#ifdef WIN32
void data_encoding_tools_test()
{
	std::vector<std::string> strings;
	std::string content = ws2s(L"我a是中b国人.");
	windows_split_utf8_string(content, strings);
	for(std::vector<std::string>::iterator it = strings.begin(); it != strings.end(); it++) {
		OutputDebugString(s2ws(*it).c_str());OutputDebugString(L"\n");
	}
	
	OutputDebugString(s2ws(append_header_to_string(content, "18665152605")).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 0)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 1)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 2)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 3)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 4)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 5)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 6)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 7)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 8)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 9)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 10)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 100)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 1000)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 10000)).c_str());OutputDebugString(L"\n");
}
#endif