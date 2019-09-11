//
//  smpp_datacoding_toolkit.h
//  CBClient
//
//  Created by Alex on 6/6/14.
//  Copyright (c) 2014 Bifang technology. All rights reserved.
//

#ifndef __CBClient__smpp_datacoding_toolkit__
#define __CBClient__smpp_datacoding_toolkit__

#include <string>
#include <vector>
// the length of ucs2 must be X4 of length of utf8
bool smpp_datacoding_convert_utf8_to_ucs2(unsigned char* utf8, unsigned char* ucs2_bytes, int& ucs2_bytes_length);
std::string smpp_datacoding_convert_ucs2_to_utf8(unsigned short *ucs2, int short_count);

bool smpp_datacoding_is_pure_ascii(const char* utf8);

void smpp_datacoding_tester();

struct splitted_sm
{
	std::string content_;
	unsigned char dc_;
	unsigned char udhi_ref_;
	unsigned char udhi_total_;
	unsigned char udhi_index_;
	splitted_sm(std::string content, unsigned char dc, unsigned char ref, unsigned char total, unsigned char index)
	{
		content_ = content;
		dc_ = dc;
		udhi_ref_ = ref;
		udhi_total_ = total;
		udhi_index_ = index;
	}
};
void smpp_datacoding_split_long_sm(const std::string& long_sm, std::vector<splitted_sm>& smss);
#endif /* defined(__CBClient__smpp_datacoding_toolkit__) */
