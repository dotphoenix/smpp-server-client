//
//  json_parser.h
//  mongoose
//
//  Created by Alex on 5/26/14.
//  Copyright (c) 2014 BiFang technology. All rights reserved.
//

#ifndef __mongoose__json_parser__
#define __mongoose__json_parser__
#include "algorithm.hpp"
#include <string>
#include <sstream>
#include <vector>
#include "base64.h"
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
class const_strings
{
public:
	static std::string USERNAME;
	static std::string SESSION_ID;
	static std::string MESSAGE_ID;
	static std::string ERRORSTATUS;
	static std::string ERRORCODE;
	static std::string DST_ADDR;
	static std::string DST_ADDRS;
	static std::string SUBMITTIME;
	static std::string DATACODING;
	static std::string CONTENT;
	static std::string DELIVERTIME;
	static std::string UDHI_REFERENCE;
	static std::string UDHI_TOTAL_PARTS;
	static std::string UDHI_PART_INDEX;
	static std::string SRC_ADDR;
	static std::string SMSS_ROOTNAME;
	static std::string GLOBAL_STATUS;
	static std::string MOS;
	static std::string EXPECT_SUBMIT_TIME;
};
class sm_info
{
public:
	sm_info(){}
	sm_info(const std::string& dst, const std::string& msg, const std::string& src,
			unsigned char datacoding, unsigned char udhi_reference,
			unsigned char udhi_total_parts, unsigned char udhi_part_index)
	{
		dst_ = dst;
		msg_ = msg;
		src_ = src;
		datacoding_ = datacoding;
		udhi_reference_ = udhi_reference;
		udhi_total_parts_ = udhi_total_parts;
		udhi_part_index_ = udhi_part_index;
	}
public:
	std::string dst_;
	std::string msg_;
	std::string src_;
	unsigned char datacoding_;
	unsigned char udhi_reference_;
	unsigned char udhi_total_parts_;
	unsigned char udhi_part_index_;
};
struct sm_status
{
public:
	sm_status(){}
	sm_status(const std::string& username, const std::string& session_id, const std::string& message_id, const std::string& status, int ec, const std::string& dst, boost::uint64_t dt, unsigned char udhi_reference,
			  unsigned char udhi_total_parts, unsigned char udhi_part_index)
	{
		username_ = username;
		session_id_ = session_id;
		message_id_ = message_id;
		status_ = status;
		error_code_ = ec;
		dst_ = dst;
		deliver_time_ = dt;
		udhi_reference_ = udhi_reference;
		udhi_total_parts_ = udhi_total_parts;
		udhi_part_index_ = udhi_part_index;
	}
public:
	std::string username_;
	std::string session_id_;
	std::string message_id_;
	std::string status_;
	int error_code_;
	std::string dst_;
	boost::uint64_t deliver_time_;
	unsigned char udhi_reference_;
	unsigned char udhi_total_parts_;
	unsigned char udhi_part_index_;
};
class sm_mo
{
public:
	sm_mo(){}
	sm_mo(const std::string& src_addr, const std::string& dst_addr, unsigned char dc_type, const std::string& content, boost::uint64_t deliver_time)
	{
		src_addr_ = src_addr;
		dst_addr_ = dst_addr;
		dc_ = dc_type;
		content_ = content;
		deliver_time_ = deliver_time;
	}
public:
	std::string src_addr_;
	std::string dst_addr_;
	unsigned char dc_;
	std::string content_;
	boost::uint64_t deliver_time_;
};
class json_parser
{
public:
	static std::string format_time_to_str(time_t t);
	static std::string format_int64_to_string(boost::uint64_t t);
	static std::string generate_smss(const std::vector<sm_info>& sis);
	static bool parse_smss(const std::string& s, std::vector<sm_info>& sis);
	
	static std::string generate_global_status(const std::vector<sm_status>& sss);
	static bool parse_global_status(const std::string& s, std::vector<sm_status>& sss);
	
	static std::string generate_mos(const std::vector<sm_mo>& mos);
	static bool parse_mos(const std::string& s, std::vector<sm_mo>& mos);
public:
	static void tester();
};
#endif /* defined(__mongoose__json_parser__) */
