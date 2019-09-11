//
//  json_parser.cpp
//  mongoose
//
//  Created by Alex on 5/26/14.
//  Copyright (c) 2014 BiFang technology. All rights reserved.
//

#include "json_parser.h"
#include <boost/progress.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "logger.hpp"
#include "cb_protocol_public.hpp"

std::string const_strings::DST_ADDR = "dst_addr";
std::string const_strings::DST_ADDRS = "dst_addrs";
std::string const_strings::CONTENT = "content";
std::string const_strings::DATACODING = "dc";
std::string const_strings::UDHI_REFERENCE = "udhi_ref";
std::string const_strings::UDHI_TOTAL_PARTS = "udhi_total";
std::string const_strings::UDHI_PART_INDEX = "udhi_index";
std::string const_strings::SMSS_ROOTNAME = "smss";
std::string const_strings::USERNAME = "username";
std::string const_strings::GLOBAL_STATUS = "global_status";
std::string const_strings::SESSION_ID = "session_id";
std::string const_strings::MESSAGE_ID = "message_id";
std::string const_strings::ERRORSTATUS = "error_status";
std::string const_strings::ERRORCODE = "error_code";
std::string const_strings::SRC_ADDR = "src_addr";
std::string const_strings::SUBMITTIME = "submit_time";
std::string const_strings::DELIVERTIME = "deliver_time";
std::string const_strings::MOS = "mos";
std::string const_strings::EXPECT_SUBMIT_TIME = "expect_submit_time";

std::string json_parser::format_time_to_str(time_t t)
{
	std::string dt1 = boost::posix_time::to_iso_string(boost::posix_time::from_time_t(t));
	char dt[64] = {0};
	strftime(dt, 63, "%Y-%m-%d %H:%M:%S", localtime(&t));
	return std::string(dt);
}

std::string json_parser::format_int64_to_string(boost::uint64_t t)
{
	char v[32] = {0};
#ifdef WIN32
	sprintf(v, "%I64d", (boost::int64_t)t);
#else
	sprintf(v, "%lld",  (long long int)t);
#endif
	return std::string(v);
}

void json_parser::tester()
{
	std::vector<sm_info> smss1;
	for(int i = 0; i < 5; i++)
	{
		std::string to = "1860000" ;
		std::string src = "96688";
		std::string msg = "这个短信发给Alex，   谢谢。 ";
		unsigned char dc = 0; //ucs2
		smss1.push_back(sm_info(to, msg, src, dc, 0, 1, 1));
	}
	std::string username = "alex";
	std::string s = generate_smss(smss1);
	std::vector<sm_info> smss2;
	parse_smss(s, smss2);
	assert(smss1.size() == smss2.size());
	
	std::vector<sm_status> statuses1;
	std::vector<sm_status> statuses2;
	std::string session_id = generate_message_id();
	for(int i = 0; i < 4; i++)
	{
		std::string message_id = std::string("MESSAGE ID");
		std::string status = "DELIVERED" ;
		int error_code = 0;
		std::string dst = "86186***";
		boost::uint64_t dt = (boost::uint64_t)time(NULL);
		std::string msg = "this is a message";
		statuses1.push_back(sm_status(username, session_id, message_id, status, error_code, dst, dt, 0x88, 1, 1));
	}
	s = generate_global_status(statuses1);
	logger_log(__FUNCTION__, LEVEL_DEBUG, "%s", s.c_str());
	
	parse_global_status(s, statuses2);
	
}
//#define ASSUME_MESSAGE_IS_THE_SAME_IN_ONE
std::string json_parser::generate_smss(const std::vector<sm_info>& smss)
{
	boost::property_tree::ptree pt_root;
	boost::property_tree::ptree children;
	for(size_t i = 0; i < smss.size(); i++)
	{
		boost::property_tree::ptree child;
		const sm_info& sms = smss.at(i);
		child.put(const_strings::DST_ADDR, sms.dst_);
		std::string msg_base64 = base64_encode((const unsigned char*)sms.msg_.c_str(), (unsigned int)sms.msg_.length());
		child.put(const_strings::CONTENT, msg_base64);
		child.put(const_strings::SRC_ADDR, sms.src_);
		child.put(const_strings::DATACODING, sms.datacoding_);
		child.put(const_strings::UDHI_REFERENCE, sms.udhi_reference_);
		child.put(const_strings::UDHI_TOTAL_PARTS, sms.udhi_total_parts_);
		child.put(const_strings::UDHI_PART_INDEX, sms.udhi_part_index_);
		children.push_back(std::make_pair("", child));
	}
	pt_root.add_child(const_strings::SMSS_ROOTNAME, children);
	std::stringstream ss;
	boost::property_tree::write_json(ss, pt_root);
	std::string s = ss.str();
	return s;
}
bool json_parser::parse_smss(const std::string& s, std::vector<sm_info>& smss)
{
	if(s.length() == 0)
	{
		return true;
	}
	std::istringstream iss;
	iss.str(s.c_str());
	boost::property_tree::ptree parser;
	try
	{
		boost::property_tree::json_parser::read_json(iss, parser);
		boost::property_tree::ptree sms_array = parser.get_child(const_strings::SMSS_ROOTNAME);
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, sms_array)
		{
			boost::property_tree::ptree p = v.second;
			std::string dst = p.get<std::string>(const_strings::DST_ADDR);
			std::string msg_base64 = p.get<std::string>(const_strings::CONTENT);
			std::vector<unsigned char> vc = base64_decode(msg_base64);
			std::string msg = std::string(vc.begin(), vc.end());
			std::string src = p.get<std::string>(const_strings::SRC_ADDR);
			unsigned char dc = (unsigned char)p.get<unsigned int>(const_strings::DATACODING);
			unsigned char udhi_ref = (unsigned char)p.get<unsigned int>(const_strings::UDHI_REFERENCE);
			unsigned char udhi_total = (unsigned char)p.get<unsigned int>(const_strings::UDHI_TOTAL_PARTS);
			unsigned char udhi_index = (unsigned char)p.get<unsigned int>(const_strings::UDHI_PART_INDEX);
			smss.push_back(sm_info(dst, msg, src, dc, udhi_ref, udhi_total, udhi_index));
		}
	}
	catch(boost::property_tree::json_parser_error& e)
	{
		logger_log(__FUNCTION__, LEVEL_ERROR,"exception: %s, value: %s", e.what(), s.c_str());
		return false;
	}
	catch(std::exception& e)
	{
		logger_log(__FUNCTION__, LEVEL_ERROR,"exception: %s, value: %s", e.what(), s.c_str());
		return false;
	}
	return true;
}

std::string json_parser::generate_global_status(const std::vector<sm_status>& sss)
{
	boost::property_tree::ptree pt_root;
	boost::property_tree::ptree children;
	
	for(size_t i = 0; i < sss.size(); i++)
	{
		boost::property_tree::ptree child;
		sm_status ss = sss.at(i);
		child.put(const_strings::USERNAME, ss.username_);
		child.put(const_strings::SESSION_ID, ss.session_id_);
		child.put(const_strings::MESSAGE_ID, ss.message_id_);
		child.put(const_strings::ERRORSTATUS, ss.status_);
		child.put(const_strings::ERRORCODE, ss.error_code_);
		child.put(const_strings::DST_ADDR, ss.dst_);
		child.put(const_strings::DELIVERTIME, format_int64_to_string(ss.deliver_time_));
		child.put(const_strings::UDHI_REFERENCE, ss.udhi_reference_);
		child.put(const_strings::UDHI_TOTAL_PARTS, ss.udhi_total_parts_);
		child.put(const_strings::UDHI_PART_INDEX, ss.udhi_part_index_);
		children.push_back(std::make_pair("", child));
	}
	pt_root.add_child(const_strings::GLOBAL_STATUS, children);
	std::stringstream ss;
	boost::property_tree::write_json(ss, pt_root);
	std::string s = ss.str();
	return s;
}
bool json_parser::parse_global_status(const std::string &s, std::vector<sm_status> &sss)
{
	if(s.length() == 0)
	{
		return true;
	}
	std::istringstream iss;
	iss.str(s.c_str());
	boost::property_tree::ptree parser;
	try
	{
		boost::property_tree::json_parser::read_json(iss, parser);
		boost::property_tree::ptree ss_array = parser.get_child(const_strings::GLOBAL_STATUS);
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v_ss, ss_array)
		{
			boost::property_tree::ptree child = v_ss.second;
			sm_status ss;
			ss.username_ = child.get<std::string>(const_strings::USERNAME);
			ss.session_id_ = child.get<std::string>(const_strings::SESSION_ID);
			ss.message_id_ = child.get<std::string>(const_strings::MESSAGE_ID);
			ss.status_ = child.get<std::string>(const_strings::ERRORSTATUS);
			ss.error_code_ = child.get<int>(const_strings::ERRORCODE);
			ss.dst_ = child.get<std::string>(const_strings::DST_ADDR);
			ss.deliver_time_ = child.get<boost::uint64_t>(const_strings::DELIVERTIME);
			ss.udhi_reference_ = (unsigned char)child.get<unsigned int>(const_strings::UDHI_REFERENCE);
			ss.udhi_total_parts_ = (unsigned char)child.get<unsigned int>(const_strings::UDHI_TOTAL_PARTS);
			ss.udhi_part_index_ = (unsigned char)child.get<unsigned int>(const_strings::UDHI_PART_INDEX);
			sss.push_back(ss);
		}
	}
	catch(boost::property_tree::json_parser_error& e)
	{
		logger_log(__FUNCTION__, LEVEL_ERROR,"exception: %s, value: %s", e.what(), s.c_str());
		return false;
	}
	catch(std::exception& e)
	{
		logger_log(__FUNCTION__, LEVEL_ERROR,"exception: %s, value: %s", e.what(), s.c_str());
		return false;
	}
	return true;
}

std::string json_parser::generate_mos(const std::vector<sm_mo>& mos)
{
	boost::property_tree::ptree pt_root;
	boost::property_tree::ptree children;
	
	for(size_t i = 0; i < mos.size(); i++)
	{
		boost::property_tree::ptree child;
		sm_mo mo = mos.at(i);
		child.put(const_strings::SRC_ADDR, mo.src_addr_);
		child.put(const_strings::DST_ADDR, mo.dst_addr_);
		child.put(const_strings::DATACODING, mo.dc_);
		std::string content_base64 = base64_encode((const unsigned char*)mo.content_.c_str(), (unsigned int)mo.content_.length());
		child.put(const_strings::CONTENT, content_base64);
		child.put(const_strings::DELIVERTIME, format_int64_to_string(mo.deliver_time_));
		children.push_back(std::make_pair("", child));
	}
	pt_root.add_child(const_strings::MOS, children);
	std::stringstream ss;
	boost::property_tree::write_json(ss, pt_root);
	std::string s = ss.str();
	return s;
}
bool json_parser::parse_mos(const std::string &s, std::vector<sm_mo> &mos)
{
	if(s.length() == 0)
	{
		return true;
	}
	std::istringstream iss;
	iss.str(s.c_str());
	boost::property_tree::ptree parser;
	try
	{
		boost::property_tree::json_parser::read_json(iss, parser);
		boost::property_tree::ptree ss_array = parser.get_child(const_strings::MOS);
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v_ss, ss_array)
		{
			boost::property_tree::ptree child = v_ss.second;
			sm_mo mo;
			mo.src_addr_ = child.get<std::string>(const_strings::SRC_ADDR);
			mo.dst_addr_ = child.get<std::string>(const_strings::DST_ADDR);
			mo.dc_ = (unsigned char)child.get<int>(const_strings::DATACODING);
			std::string content_base64 = child.get<std::string>(const_strings::CONTENT);
			std::vector<unsigned char> vc = base64_decode(content_base64);
			mo.content_ = std::string(vc.begin(), vc.end());
			mo.deliver_time_ = child.get<boost::uint64_t>(const_strings::DELIVERTIME);
			mos.push_back(mo);
		}
	}
	catch(boost::property_tree::json_parser_error& e)
	{
		logger_log(__FUNCTION__, LEVEL_ERROR,"exception: %s, value: %s", e.what(), s.c_str());
		return false;
	}
	catch(std::exception& e)
	{
		logger_log(__FUNCTION__, LEVEL_ERROR,"exception: %s, value: %s", e.what(), s.c_str());
		return false;
	}
	return true;
}
