//
//  smpp_datacoding_toolkit.cpp
//  CBClient
//
//  Created by Alex on 6/6/14.
//  Copyright (c) 2014 Bifang technology. All rights reserved.
//

#include "smpp_datacoding_toolkit.h"
#include <string.h>
#include <boost/shared_ptr.hpp>
#include <assert.h>
#include "memory_buffer.hpp"
#include "logger.hpp"
#include <time.h>
#include <vector>
static bool utf8_chars_to_ucs2(unsigned short& us, const unsigned char * input, const unsigned char ** end_ptr)
{
    *end_ptr = input;
    if (input[0] == 0)
	{
        return false;
	}
    if (input[0] < 0x80)
	{
        *end_ptr = input + 1;
        us = input[0];
		return true;
    }
    if ((input[0] & 0xE0) == 0xE0)
	{
        if (input[1] == 0 || input[2] == 0)
		{
			return false;
		}
        *end_ptr = input + 3;
        us = (input[0] & 0x0F)<<12 | (input[1] & 0x3F)<<6 | (input[2] & 0x3F);
		return true;
    }
    if ((input[0] & 0xC0) == 0xC0)
	{
        if (input[1] == 0)
		{
            return false;
		}
        *end_ptr = input + 2;
		{
			us = (input[0] & 0x1F)<<6 | (input[1] & 0x3F);
			return true;
		}
    }
    return false;
}
static void convert_utf8_bytes_to_ucs2(const char* utf8, unsigned short* ucs2, int& ucs2_length)
{
	const unsigned char * p1 = (const unsigned char *)utf8;
	const unsigned char * p2 = p1;
	int index = 0;
	while(*p1 != '\0')
	{
		unsigned short us;
		if(utf8_chars_to_ucs2(us, p1, &p2))
		{
			ucs2[index] = us;
			index++;
		}
		if(*p2 == '\0')
		{
			ucs2_length = index;
			break;
		}
		else
		{
			p1 = p2;
		}
	}
	ucs2_length = index;
}
static int convert_ucs2_to_utf8_chars(int ucs2, unsigned char * utf8)
{
    if (ucs2 < 0x80)
	{
        utf8[0] = ucs2;
        utf8[1] = '\0';
        return 1;
    }
    if (ucs2 >= 0x80  && ucs2 < 0x800)
	{
        utf8[0] = (ucs2 >> 6)   | 0xC0;
        utf8[1] = (ucs2 & 0x3F) | 0x80;
        utf8[2] = '\0';
        return 2;
    }
    if (ucs2 >= 0x800 && ucs2 < 0xFFFF)
	{
		if (ucs2 >= 0xD800 && ucs2 <= 0xDFFF)
		{
			/* Ill-formed. */
			return 0;
		}
        utf8[0] = ((ucs2 >> 12)       ) | 0xE0;
        utf8[1] = ((ucs2 >> 6 ) & 0x3F) | 0x80;
        utf8[2] = ((ucs2      ) & 0x3F) | 0x80;
        utf8[3] = '\0';
        return 3;
    }
    if (ucs2 >= 0x10000 && ucs2 < 0x10FFFF)
	{
		/* http://tidy.sourceforge.net/cgi-bin/lxr/source/src/utf8.c#L380 */
		utf8[0] = 0xF0 | (ucs2 >> 18);
		utf8[1] = 0x80 | ((ucs2 >> 12) & 0x3F);
		utf8[2] = 0x80 | ((ucs2 >> 6) & 0x3F);
		utf8[3] = 0x80 | ((ucs2 & 0x3F));
        utf8[4] = '\0';
        return 4;
    }
    return 0;
}
static void change_endian_1(unsigned short us, unsigned char& c1, unsigned char& c2)
{
	unsigned char* c11 = (unsigned char*)&us;
	unsigned char* c22 = c11 + 1;
	c1 = *c11;
	c2 = *c22;
}
static void change_endian_2(unsigned short us, unsigned short& us2)
{
	unsigned char c1, c2;
	unsigned char* c11 = (unsigned char*)&us;
	unsigned char* c22 = c11 + 1;
	c1 = *c11;
	c2 = *c22;
	us2 = (c1 << 8) + c2;
}
bool smpp_datacoding_convert_utf8_to_ucs2(unsigned char* utf8, unsigned char* ucs2_bytes, int& ucs2_bytes_length)
{
	int utf8_len = (int)strlen((const char*)utf8);
	ucs2_bytes_length = 0;
#ifdef DEBUG_CONVERTER
	char tmp[256] = {0};
	sprintf(tmp, "length:%d", utf8_len);
	std::string s_tmp = tmp;
	for(int i = 0; i < utf8_len; i++)
	{
		sprintf(tmp, " 0x%2x", utf8[i]);
		s_tmp += tmp;
	}
	logger_log(__FUNCTION__, LEVEL_WARNING, "***** %s", s_tmp.c_str());
#endif
	boost::shared_ptr<unsigned short> ucs2_short;
	ucs2_short.reset(new unsigned short[utf8_len * 4 + 1]);
	memset(ucs2_short.get(), 0, sizeof(short) * (utf8_len * 4 + 1));
	int ucs2_length = 0;
	convert_utf8_bytes_to_ucs2((const char*)utf8, ucs2_short.get(), ucs2_length);
	unsigned char c1, c2;
	for(int i = 0; i < ucs2_length; i++)
	{
		change_endian_1(ucs2_short.get()[i], c1, c2);
		ucs2_bytes[i * 2 ] = c2;
		ucs2_bytes[i * 2 + 1] = c1;
	}
	ucs2_bytes_length = ucs2_length * 2;
	return true;
}

std::string smpp_datacoding_convert_ucs2_to_utf8(unsigned short *ucs2, int short_count)
{
    unsigned short unicode;
    unsigned char bytes[8] = {0};
    int i = 0, j = 0;
    int utf8_len = 0;
	memory_buffer_ptr bp;
	bp.reset(new memory_buffer(short_count * 4 + 32));
	for (i = 0; i < short_count; i++)
	{
		change_endian_2(ucs2[i], unicode);
		int nbytes = convert_ucs2_to_utf8_chars(unicode, bytes);
		for (j = 0; j < nbytes; j++)
		{
			bp->set_data(utf8_len, bytes + j, sizeof(unsigned char));
			utf8_len++;
		}
	}
	return std::string((char*)bp->data());
}

bool smpp_datacoding_is_pure_ascii(const char* utf8)
{
	int len = (int)strlen(utf8);
	for( int i = 0 ; i < len ; i++)
	{
		if((unsigned char)utf8[i] >= 0xa0)
		{
			return false;
		}
	}
	return true;
}

void smpp_datacoding_tester()
{
	bool b = smpp_datacoding_is_pure_ascii((const char*)"中国");
	assert(!b);
	b = smpp_datacoding_is_pure_ascii((const char*)"_1 ab*K");
	assert(b);
	b = smpp_datacoding_is_pure_ascii((const char*)"，，_1 ab*K");
	assert(!b);
}

//默认只要含有中文字符，就会按照UCS编码
//使用UCS2时长短信拆分后每条短信的字数
static int MAX_MULTIPART_MSG_SEGMENT_SIZE_UCS2 = 67;
//使用UCS2时非长短信每条的长度
static int MAX_SINGLE_MSG_SEGMENT_SIZE_UCS2 = 70;

//使用7bit编码时长短信拆分后每条短信的字数
static int MAX_MULTIPART_MSG_SEGMENT_SIZE_7BIT = 153;
//使用7bit编码时非长短信每条短信的字数
static int MAX_SINGLE_MSG_SEGMENT_SIZE_7BIT = 160;

void smpp_datacoding_split_long_sm(const std::string& long_sm, std::vector<splitted_sm>& smss)
{
	// configure variables acording to if message contains national
	// characters
	int max_single_sm_length = 0;
	int max_multipart_sm_segment_size = 0;
	unsigned char dc;
	unsigned char ucs2_bytes[1024 * 64] = {0};
	unsigned short* ucs2_short = NULL;
	int ucs2_bytes_length = 0;
	if (smpp_datacoding_is_pure_ascii(long_sm.c_str()))
	{
		dc = 0;
		max_single_sm_length = MAX_SINGLE_MSG_SEGMENT_SIZE_7BIT;
		max_multipart_sm_segment_size = MAX_MULTIPART_MSG_SEGMENT_SIZE_7BIT;
	}
	else
	{
		dc = 8;
		max_single_sm_length = MAX_SINGLE_MSG_SEGMENT_SIZE_UCS2;
		max_multipart_sm_segment_size = MAX_MULTIPART_MSG_SEGMENT_SIZE_UCS2;
		smpp_datacoding_convert_utf8_to_ucs2((unsigned char*)long_sm.c_str(), ucs2_bytes, ucs2_bytes_length);
		ucs2_short = (unsigned short*)ucs2_bytes;
	}
	
	int number_of_segments;
	int message_length;
	if(dc == 0)
	{
		number_of_segments = (int)(long_sm.length() / max_multipart_sm_segment_size);
		message_length = (int)long_sm.length();
	}
	else
	{
		number_of_segments = (int)((ucs2_bytes_length>>1) / max_multipart_sm_segment_size);
		message_length = (int)(ucs2_bytes_length>>1);
	}
	if (number_of_segments > 255)
	{
		number_of_segments = 255;
		message_length = number_of_segments * max_multipart_sm_segment_size;
	}
	if ((message_length % max_multipart_sm_segment_size) > 0)
	{
		number_of_segments++;
	}
	
	if(dc == 0)
	{
		if (message_length > max_single_sm_length)
		{
			std::string long_content = long_sm;
			static int i = 0;
			srand((unsigned int)(time(NULL) + i++));
			int ref = (int)((rand() + long_content.length()) % 0xFF);
			for(int i = 0; i < number_of_segments; i++)
			{
				std::string content;
				int begin_index = i * max_multipart_sm_segment_size;
				if(i == number_of_segments - 1)
				{
					content = long_content.substr(begin_index, long_content.length()); //it is ok here.
				}
				else
				{
					content = long_content.substr(begin_index, max_multipart_sm_segment_size);
				}
				
				smss.push_back(splitted_sm(content, dc, (unsigned char)ref, (unsigned char)number_of_segments, (unsigned char)(i + 1)));
				//logger_log(__FUNCTION__, LEVEL_DEBUG, "content: %s", content.c_str());
			}
		}
		else
		{
			smss.push_back(splitted_sm(long_sm, dc, (unsigned char)0, (unsigned char)1, (unsigned char)1));
			//logger_log(__FUNCTION__, LEVEL_DEBUG, "content: %s", long_sm.c_str());
		}
	}
	else
	{
		if (message_length > max_single_sm_length)
		{
			unsigned short* short_content = NULL;
			srand((unsigned int)time(NULL));
			int ref = (int)(rand() % 0xFF);
			for(int i = 0; i < number_of_segments; i++)
			{
				std::string content;
				int begin_index = i * max_multipart_sm_segment_size;
				int end_index = 0;
				if(i == number_of_segments - 1)
				{
					short_content = ucs2_short + begin_index;
					content = smpp_datacoding_convert_ucs2_to_utf8(short_content, (message_length - begin_index));
				}
				else
				{
					end_index = begin_index + max_multipart_sm_segment_size;
					short_content = ucs2_short + begin_index;
					content = smpp_datacoding_convert_ucs2_to_utf8(ucs2_short, max_multipart_sm_segment_size);
				}
				//logger_log(__FUNCTION__, LEVEL_DEBUG, "content: %s", content.c_str());
				smss.push_back(splitted_sm(content, dc, (unsigned char)ref, (unsigned char)number_of_segments, (unsigned char)(i + 1)));
			}
		}
		else
		{
			smss.push_back(splitted_sm(long_sm, dc, (unsigned char)0, (unsigned char)1, (unsigned char)1));
			//logger_log(__FUNCTION__, LEVEL_DEBUG, "content: %s", long_sm.c_str());
		}
	}
}
#ifndef WIN32
void smpp_client_datacoding_split_tester()
{
	std::string long_sm_ucs = "新年喜“羊羊”，存款到工行！工商银行深圳分行新推个人节节高存款增值服务，满足约定金额和约定存期，即可享受分段组合计息服务，存取灵活似活期、利息上浮同定期，存款利率最高可上浮20%，使您的存款利息最优化，详情咨询深圳分行各网点理财客户经理。";
	std::vector<splitted_sm> smss;
	smpp_datacoding_split_long_sm(long_sm_ucs, smss);
	
	std::string short_sm_ucs = "中华人民共和国国庆！。";
	smss.clear();
	smpp_datacoding_split_long_sm(short_sm_ucs, smss);
	
	std::string long_sm_ascii = "abcdefghijklmnopqrstuvwxyz1234567890abcdefghijklmnopqrstuvwxyz1234567890abcdefghijklmnopqrstuvwxyz1234567890abcdefghijklmnopqrstuvwxyz1234567890abcdefghijklmnopqrstuvwxyz1234567890";
	smss.clear();
	smpp_datacoding_split_long_sm(long_sm_ascii, smss);
	
	std::string short_sm_ascii = "ABCDEFGHIJKLMNOPQRST";
	smss.clear();
	smpp_datacoding_split_long_sm(short_sm_ascii, smss);
}
#endif
