/*
 *  algorithm.hpp
 *  CBPlatform
 *
 *  Created by Zhu Nengjie on 7/31/13.
 *  Copyright 2013 Bifang technology. All rights reserved.
 *
 */

#ifndef PUBLIC_ALGORITHM_TOOLKIT_H
#define PUBLIC_ALGORITHM_TOOLKIT_H
#include <boost/asio.hpp>
#include <list>
#include <boost/filesystem/convenience.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include "logger.hpp"
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string.hpp>
#include "memory_buffer.hpp"
//return the index, -1 means not exist
template<class T1, class T2>
int exist_in_vector(const std::vector<T1>& vs, const T2& s)
{
	for(size_t i = 0; i < vs.size(); i++)
	{
		T1 t1 = vs.at(i);
		if(t1 == s)
		{
			return (int)i;
		}
	}
	return -1;
}

template<class T>
bool get_iterator_of_index( T& t1, int index, typename T::iterator& it)
{
	it = t1.begin();
	size_t s = t1.size();
	if(index >= (int)s)
	{
		return false;
	}
	if(index > 0)
	{
		std::advance(it, index);
	}
	return true;
}

template<class T1, class T2>
T2 concatenate_with_separator(const T1& t1, const T2& sep)
{
	T2 s;
	typename T1::const_iterator it;
	for (it = t1.begin(); it != t1.end(); it++)
	{
		if(it != t1.begin())
			s += sep;
		std::stringstream ss;
		ss<<*it;
		s += ss.str();
		
	}
	return s;
}
template<class T1, class T2>
bool split_by_separator(const T1& s, const T1& sep,  T2& values)
{
	if(s.length() > 0)
	{
		std::vector<T1> vs;
		//boost::split(vs, s, boost::algorithm::is_any_of(sep), boost::algorithm::token_compress_on);
		boost::split_regex(vs, s, boost::regex(sep));
		for(size_t i = 0; i < vs.size(); i++)
		{
			std::stringstream ss;
			ss<<vs.at(i);
			typename T2::value_type t;
			ss>>t;
			values.push_back(t);
		}
	}
	return true;
}

inline bool read_line(std::ifstream& ifs, char* line, size_t length)
{
	if(ifs.getline(line, length))
	{
		size_t line_length = strlen(line);
		if(line[line_length - 1] == '\r')
		{
			line[line_length - 1] = '\0';
		}
		if(line[line_length - 1] == '\n')
		{
			line[line_length - 1] = '\0';
		}
		if((line[line_length - 2] == '\r') && (line[line_length - 1] == '\n') )
		{
			line[line_length - 2] = '\0';
		}
		return true;
	}
	return false;
}

inline void write_line(std::ofstream& ofs, const char* line, size_t length)
{
#ifdef WIN32
	std::string CRLF = "\n";
#elif defined __APPLE__
	std::string CRLF = "\r";
#else
	std::string CRLF = "\n";
#endif
	
	ofs.write(line, length);
	ofs.write(CRLF.c_str(), CRLF.length());
	ofs.flush();
}

#define WORK_START \
boost::posix_time::ptime time_start = boost::posix_time::microsec_clock::local_time();  

#define WORK_FINISH(work_name) \
boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();   \
boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse = now - time_start; \
boost::uint64_t millisec = time_elapse.total_milliseconds();\
if(millisec >= 200)\
{\
std::stringstream ss;\
ss<<work_name<<" costs ";\
ss<<millisec<<" milliseconds";\
logger_log(__FUNCTION__, LEVEL_WARNING, ss.str().c_str() );\
}

#define TEST_WORK_START(index) \
logger_log(__FUNCTION__, LEVEL_WARNING, "thread %d start...",index);\
boost::posix_time::ptime time_start = boost::posix_time::microsec_clock::local_time();  

#define TEST_WORK_FINISH(work_name) \
boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();   \
boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse = now - time_start; \
boost::uint64_t millisec = time_elapse.total_milliseconds();\
{\
	std::stringstream ss;\
	ss<<work_name<<" costs ";\
	ss<<millisec<<" milliseconds";\
	logger_log(__FUNCTION__, LEVEL_WARNING, ss.str().c_str() );\
}

#ifdef WIN32
inline std::string ws2s(const std::wstring& utf16)
{
	LPSTR pszUtf8 = NULL;
	std::string utf8;

	if (utf16.length() == 0)
	{
		return utf8; 
	}

	size_t nLen16 = utf16.length();
	size_t nLen8 = 0;

	if ((nLen8 = WideCharToMultiByte (CP_UTF8, 0, utf16.c_str(), (int)nLen16, 
		NULL, 0, 0, 0) + 2) == 2)
	{
		return utf8; 
	}

	pszUtf8 = new char [nLen8 + 2];
	if (pszUtf8)
	{
		memset (pszUtf8, 0x00, nLen8 + 2);
		WideCharToMultiByte(CP_UTF8, 0, utf16.c_str(), (int)nLen16, pszUtf8, (int)nLen8, 0, 0);
		utf8 = std::string(pszUtf8);
	}

	delete [] pszUtf8;
	return utf8; //utf8 encoded string
}

inline std::wstring s2ws(const std::string& utf8)
{
	LPWSTR pszUtf16 = NULL;
	std::wstring utf16;

	if (utf8.length() == 0)
	{
		return utf16; 
	}

	size_t nLen8 = utf8.length();
	size_t nLen16 = 0;

	if ((nLen16 = MultiByteToWideChar (CP_UTF8, 0, utf8.c_str(), (int)nLen8, NULL, 0)) == 0)
	{
		return utf16;
	}
	pszUtf16 = new wchar_t[nLen16 + 2];
	if (pszUtf16)
	{
		wmemset (pszUtf16, 0x00, nLen16 + 2);
		MultiByteToWideChar (CP_UTF8, 0, utf8.c_str(), (int)nLen8, pszUtf16, (int)nLen16);
		utf16 = std::wstring(pszUtf16);
	}
	delete [] pszUtf16;
	return utf16; //utf16 encoded string
}
#else
inline std::string ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, "");        // curLocale = "C";
#ifdef WIN32
	setlocale(LC_ALL, "chs");
#elif __APPLE__
	setlocale(LC_ALL, "en_US.UTF-8");
#elif LINUX_CENTOS
	setlocale(LC_ALL, "en_US.UTF-8");
#else
	setlocale(LC_ALL, "en_US.UTF-8");
#endif
	const wchar_t* wsource = ws.c_str();
	size_t size = 4 * ws.size() + 4; 
	char *dest = new char[size];
	memset(dest, 0, size);
	wcstombs(dest, wsource, size);
	std::string result = dest;
	delete[] dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

inline std::wstring s2ws(const std::string& s)
{
	std::string curLocale = setlocale(LC_ALL, "");        // curLocale = "C";
#ifdef WIN32
	setlocale(LC_ALL, "chs");
#elif __APPLE__
	setlocale(LC_ALL, "en_US.UTF-8");
	//setlocale(LC_ALL, "zh_CN.GB18030");
#elif LINUX_CENTOS
	setlocale(LC_ALL, "en_US.UTF-8");
#else
	setlocale(LC_ALL, "en_US.UTF-8");
#endif
	//size_t size1 = s.length();
	const char* source = s.c_str();
	//size_t size2 = strlen(source);
	size_t size = s.size() + 2;
	wchar_t *wdest = new wchar_t[size];
	wmemset(wdest, 0, size);
	mbstowcs(wdest, source, size);
	std::wstring result = wdest;
	delete []wdest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}
#endif
//-------------------------------------------------------------------------
// Inline functions that convert to/from host and network byte order.
//-------------------------------------------------------------------------
	
/// @brief Returns true if the current machine is big endian.
/// @return true if big endian or false if little endian.
inline bool is_big_endian() // Big Endian is network byte order.
{
	const long x = 1;
	return *(const char*)&x ? false : true;
}
	
/// @brief Converts a 32 bit integer to network byte order.
/// @param n The integer in host byte order.
/// @return The integer in network byte order.
inline unsigned int ntoh32(const unsigned int& n)
{
	if(is_big_endian())
		return n;
	return (n & 0x000000ff) << 24 |
	(n & 0x0000ff00) << 8 |
	(n & 0x00ff0000) >> 8 |
	(n & 0xff000000) >> 24;
}
	
/// @brief Converts a 32 bit integer to host byte order.
/// @param n The integer in network byte order.
/// @return The integer in host byte order.
inline unsigned int hton32(const unsigned int& n)
{
	if(is_big_endian())
		return n;
	return (n & 0x000000ff) << 24 |
	(n & 0x0000ff00) << 8 |
	(n & 0x00ff0000) >> 8 |
	(n & 0xff000000) >> 24;
}
	
/// @brief Converts a 16 bit integer to network byte order.
/// @param n The integer in host byte order.
/// @return The integer in network byte order.
inline unsigned short ntoh16(const unsigned short& n)
{
	if(is_big_endian())
		return n;
	return (n & 0x00ff) << 8 | (n & 0xff00) >> 8;
}
	
/// @brief Converts a 16 bit integer to host byte order.
/// @param n The integer in network byte order.
/// @return The integer in host byte order.
inline unsigned short hton16(const unsigned short& n)
{
	if(is_big_endian())
		return n;
	return (n & 0x00ff) << 8 | (n & 0xff00) >> 8;
}

/*
format unsigned int to format as: 00001, 00100,
such as v = 9, length = 8, the return is 00000009
such as v = 9, length = 832, the return is 00000832
*/
inline std::string format_uint32_to_lenght(unsigned int v, int length)
{
	std::stringstream ss;
	ss<<std::setw(length)<<std::setfill('0')<<v;
	return ss.str();
}

template<class out_type,class in_value>
out_type datatype_convert(const in_value & t)
{
	std::stringstream ss;
	if (typeid(t) == typeid(double))
	{
		ss << std::setprecision(12) <<t;
	}
	else
	{
		ss<<t;
	}
	out_type result;
	ss>>result;
	return result;
}
/*
on visual studio,if encounter compile error when compiling a cpp file:
fatal error C1189: #error :  WinSock.h has already been included
you can put #include "algorithm.hpp" or #include <boost/asio.hpp> as the first line of that cpp file
*/
inline std::string uint_ip_2_str(unsigned int uip)
{
	
		char sip[32];
		boost::system::error_code ec;
		uip = ntohl(uip);
		boost::asio::detail::socket_ops::inet_ntop(AF_INET, &uip, sip, 32, 0, ec);
		return std::string(sip);
}

inline unsigned int str_ip_2_uint(const std::string& sip)
{
	boost::system::error_code ec;
	unsigned int uip;
	boost::asio::detail::socket_ops::inet_pton(AF_INET, sip.c_str(), &uip, NULL, ec);
	return htonl(uip);
}

inline std::string ushort_port_2_str(unsigned short port)
{
	std::stringstream ss;
	ss<<port;
	std::string s = ss.str();
	return s;
}

inline int compare_date(boost::uint64_t t1, boost::uint64_t t2)
{
	time_t tt1 = (time_t)t1;
	time_t tt2 = (time_t)t2;
	int year1, year2, mon1, mon2, day1, day2;
	tm* ptm;
	ptm = localtime(&tt1);
	year1 = ptm->tm_year;
	mon1 = ptm->tm_mon;
	day1 = ptm->tm_mday;

	ptm = localtime(&tt2);
	year2 = ptm->tm_year;
	mon2 = ptm->tm_mon;
	day2 = ptm->tm_mday;
	if(year1 > year2)
	{
		return 1;
	}
	else if(year1 < year2)
	{
		return -1;
	}
	else
	{
		if(mon1 > mon2)
		{
			return 1;
		}
		else if(mon1 < mon2)
		{
			return -1;
		}
		else
		{
			if(day1 > day2)
			{
				return 1;
			}
			else if(day1 < day2)
			{
				return -1;
			}
			else
			{
				return 0;
			}
		}
	}
}

inline bool string_ends_with(const std::string& s1, const std::string& s2)
{
	return boost::ends_with(s1, s2);
}
inline bool string_starts_with(const std::string& s1, const std::string& s2)
{
	return boost::starts_with(s1, s2);
}

template <class datatype>
class remove_elements_match  
{  
public:  
	remove_elements_match(const datatype& dt)
	{
		dt_ = dt;
	}
	bool operator()(const datatype& dt)  
	{  
		return dt_ == dt;  
	}  
	datatype dt_;
};  
template <class datatype>
void remove_elements(std::list<datatype>& l, std::vector<datatype>& elements)
{
	for(size_t i = 0; i < elements.size(); i++)
	{
		remove_elements_match<datatype> match(elements.at(i));
		l.remove_if(match);
	}
}

inline bool is_not_alnum(char c)
{
	return ((isalpha(c) == 0) && (isdigit(c) == 0) && (c != '_') && (c != '-'));
}
inline bool string_is_alnum_only(const std::string& s)
{
	if(s.length() == 0)
	{
		return false;
	}
	bool b = (find_if(s.begin(), s.end(), is_not_alnum) == s.end());
	return b;
}
inline bool is_not_digit(char c)
{
	return (isdigit(c) == 0);
}
inline bool string_is_number_only(const std::string& s)
{
	std::string tmp = s;
	if(tmp.length() > 0)
	{
		if(isdigit(s.at(0)) == 0) //first char can be -1
		{
			if(s.at(0) != '-')
			{
				return false;
			}
			else
			{
				tmp = s.substr(1, s.length() - 1);
			}
		}
	}
	else
	{
		return false;
	}
	bool b = (find_if(tmp.begin(), tmp.end(), is_not_digit) == tmp.end());
	return b;
}


inline int compare_string_nocase(const std::string& s1, const std::string& s2)
{
	std::string l_s1 = s1, l_s2 = s2;
	boost::to_lower(l_s1);
	boost::to_lower(l_s2);
	return l_s1.compare(l_s2);
}
inline int compare_trimed_string_nocase(const std::string& s1, const std::string& s2)
{
	std::string l_s1 = s1, l_s2 = s2;
	boost::to_lower(l_s1);
	boost::to_lower(l_s2);
	boost::trim(l_s1);
	boost::trim(l_s2);
	return l_s1.compare(l_s2);
}
/*
 3954580968 <--> EBB61DE8
 */
inline std::string format_decimal_string_to_hex_string(const std::string& decimal)
{
	unsigned int dec = datatype_convert<unsigned int>(decimal);
	char hex[16] = {0};
	sprintf(hex, "%x", dec);
	std::string s = hex;
	boost::to_upper(s);
	return s;
}
	/*
	 if len > 0, if the length of decimal string is shorter then the value of len,
	 we will padding 0 before decimal string.
	 */
inline std::string format_hex_string_to_decimal_string(const std::string& hex, int len = 0)
{
	unsigned int dec;
	std::stringstream ss;
	ss << std::hex << hex;
	ss >> dec;
	std::string decimal = datatype_convert<std::string>(dec);
	if(len > 0)
	{
		if((int)decimal.length() < len)
		{
			int padding_len = len - (int)decimal.length();
			for(int i = 0; i < padding_len; i++)
			{
				decimal = std::string("0") + decimal;
			}
		}
	}
	return decimal;
}

/*
 be sure the vector is sorted (can call std::sort(v.begin(), v.end()))
 
 list can call: 
 list l;
 l.sort();
 l.unique();
 */
template<class T>
inline void unique_vector(std::vector<T>& v)
{
	typename std::vector<T>::iterator it_last = std::unique(v.begin(), v.end());
	v.erase(it_last, v.end());
}
	
/*
 
 because datatype_convert may including format (such as 12,345,678) in windows platform
 */
inline std::string format_decimal_to_string(unsigned int n)
{
	char str[64] = {0};
	sprintf(str, "%u", n);
	return std::string(str);
}

inline bool double_equal(double d1, double d2)
{
	if(fabs(d1 - d2) < 0.000001)
	{
		return true;
	}
	return false;
}
#endif
