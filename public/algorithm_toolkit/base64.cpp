/* 
   base64.cpp and base64.h

   Copyright (C) 2004-2008 Ren√© Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   Ren√© Nyffenegger rene.nyffenegger@adp-gmbh.ch

   dotphoenix£®Bifang technology) modified at 20140220:
   change the decode out put to std::vector
*/

#include "base64.h"
#include <iostream>
#ifdef _MSC_VER
#pragma warning (disable : 4267)
#endif
static const std::string base64_chars_standard =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const std::string base64_chars_not_standard =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._";

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '.') || (c == '_'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len, bool using_standard)
{
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];
	while (in_len--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;
			
			for(i = 0; (i <4) ; i++)
			{
				if(using_standard)
				{
					ret += base64_chars_standard[char_array_4[i]];
				}
				else
				{
					ret += base64_chars_not_standard[char_array_4[i]];
				}
			}
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
		{
			char_array_3[j] = '\0';
		}

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
		{
			if(using_standard)
			{
				ret += base64_chars_standard[char_array_4[j]];
			}
			else
			{
				ret += base64_chars_not_standard[char_array_4[j]];
			}
		}

		while((i++ < 3))
		{
			ret += '=';
		}
	}
	
	return ret;
}
//#define DEBUG_OUTPUT
#ifdef DEBUG_OUTPUT
#include "algorithm.hpp"
#include <time.h>
#include <fstream>
#endif

std::vector<unsigned char> base64_decode(std::string const& encoded_string, bool using_standard)
{
	int in_len = (int)encoded_string.length();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::vector<unsigned char> ret;
	
	#ifdef DEBUG_OUTPUT

	unsigned int t= (unsigned int)time(NULL);
	std::string filename = datatype_convert<std::string>(t);
	std::string full_path = std::string("h:\\") + filename + ".txt";
	std::ofstream f(full_path.c_str(), std::ios::out);
	if(f.is_open())
	{
		size_t size = encoded_string.length();
		f.write((char*)encoded_string.c_str(), size);
		f.close();
	}
	#endif
	while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
	{
		char_array_4[i++] = encoded_string[in_];
		in_++;
		if (i ==4)
		{
			for (i = 0; i <4; i++)
			{
				if(using_standard)
				{
					char_array_4[i] = base64_chars_standard.find(char_array_4[i]);
				}
				else
				{
					char_array_4[i] = base64_chars_not_standard.find(char_array_4[i]);
				}
			}
			
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
			
			for (i = 0; (i < 3); i++)
			{
				ret.push_back(char_array_3[i]);
			}
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j <4; j++)
		{
			char_array_4[j] = 0;
		}
		
		for (j = 0; j <4; j++)
		{
			if(using_standard)
			{
				char_array_4[j] = base64_chars_standard.find(char_array_4[j]);
			}
			else
			{
				char_array_4[j] = base64_chars_not_standard.find(char_array_4[j]);
			}
		}
		
		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
		
		for (j = 0; (j < i - 1); j++)
		{
			ret.push_back(char_array_3[j]);
		}
	}

  return ret;
}

#if defined CB_SERVER || defined CB_AGENT_SERVER
#include <assert.h>
#include "algorithm.hpp"
#include "memory_buffer.hpp"
#include "cb_public_functions.hpp"
void base64_tester2();
void base64_tester()
{
#define BINAY_LENGTH 17
	unsigned char binary_data[BINAY_LENGTH] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x11, 0x3A, 0x4B, 0x5C, 0x6D, 0x7E, 0x8F, 0xA0, 0xFE, 0xFF};
	std::string base64 = base64_encode(binary_data, BINAY_LENGTH);
	std::vector<unsigned char> binary = base64_decode(base64);
	assert(binary.size() == BINAY_LENGTH);
	assert(binary[0] == binary_data[0]);
	assert(binary[BINAY_LENGTH - 1] == binary_data[BINAY_LENGTH - 1]);
	base64_tester2();
}
void base64_tester2()
{
	std::string image_file_path = "h:\\IMG_3408.JPG";
	std::string image_file_path_copy = "h:\\IMG_3408_copy.JPG";
	memory_buffer_ptr bp_image;
	read_binay_file(image_file_path, bp_image);
	std::string base64 = base64_encode(bp_image->data(), (unsigned int)bp_image->size());
	std::vector<unsigned char> binary = base64_decode(base64);
	memory_buffer_ptr bp_image_copy;
	bp_image_copy.reset(new memory_buffer(binary.size()));
	bp_image_copy->set_data(0, binary.data(), binary.size());
	write_binary_file(image_file_path_copy, bp_image_copy);
	assert(*(bp_image->data()) == binary.at(0));
}
#endif
