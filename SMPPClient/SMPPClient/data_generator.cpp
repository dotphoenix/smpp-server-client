//
//  main.cpp
//  SMPPClient
//
//  Created by Alex on 12/16/14.
//  Copyright (c) 2014 BiFang technology. All rights reserved.
//
#include <boost/asio/signal_set.hpp>
#include "logger.hpp"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "cb_client_api.h"
#include "json_parser.h"
#include "short_message_db.h"
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include "md5.h"
#include "smpp_datacoding_toolkit.h"
#include <boost/thread.hpp>
#include "configer.hpp"

static bool stop = false;
static boost::shared_ptr<boost::thread> simulate_thread;

static bool s_auto_submit = false;
static int s_total_to_submit = 0;
static int s_total_submitted = 0;
static int s_total_mos = 0;
static void read_auto_submit(const std::string&configer_file)
{
	if(boost::filesystem::exists(configer_file))
	{
		configer conf(configer_file);
		std::string sec_name = "auto_submit";
		s_auto_submit = (conf.get_int(sec_name, "auto_submit", 0) == 1);
		s_total_to_submit = conf.get_int(sec_name, "total_to_submit", 0);
	}
	else
	{
		s_auto_submit = false;
		s_total_to_submit = 0;
	}
}


static std::string generate_numbers(int to_generate)
{
	static int start = 0;
	std::string prefix = "8528";
	std::string numbers = "";
	for(int i = 0; i < to_generate; i++)
	{
		numbers += (prefix + format_uint32_to_lenght((unsigned int)start, 7));
		if(i != to_generate - 1)
		{
			numbers += ";";
		}
		start++;
		if(start >= 9999999)
		{
			start = 0;
		}
	}
	return numbers;
}
static int generate_smis_simulator(int to_generate, const std::string& src_addr)
{
	static unsigned int count = 0;
	std::string content;
	int index = count % 4;
	//error C2001: 常量中有换行符 --> 原因： 非BOM的UTF-8格式 
	//最简单的解决办法：偶数中文 或 结尾加英文的符号，如"."
	//合理的解决办法：字符串放到resource里面或者文件里面

	if(index == 0)
	{
		content = "this is a @ message";
	}
	else if(index == 1)
	{
		content = "这是一条@消息.";
	}
	else if(index == 2)
	{
		content = "這是一@條消息.";
	}
	else
	{
		content = "this is a long message，这是一条长消息，這是一@條长消息，这是一条将被拆分成两条消息的长消息.";
	}
	std::string numbers = generate_numbers(to_generate);
	std::vector<std::string> jsons;
	int count_of_batch = 50;
	int total = cb_client_format_sms(numbers, content, src_addr, count_of_batch, jsons);
	std::vector<std::string> session_ids;
	for(int i = 0; i < (int)jsons.size(); i++)
	{
		std::string session_id = cb_client_submit_sms(jsons.at(i));
		session_ids.push_back(session_id);
		if(session_id.length() == 0)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "add smis failed.");
		}
	}
	
	count++;
	return total;
}
static void print_dlrs(const std::vector<sm_status> & sss)
{
	for(size_t i = 0; i < sss.size(); i++)
	{
		const sm_status ss = sss.at(i);
		logger_log(__FUNCTION__, LEVEL_DEBUG, "session:%s dst:%s status:%s", ss.session_id_.c_str(), ss.dst_.c_str(), ss.status_.c_str());
	}
}
static int query_dlrs_simulator()
{
	std::vector<sm_status> sss;
	std::string json;
	int to_get  = 50;
	cb_client_get_dlrs(to_get, json);
	int count = 0;
	if(json.length() > 0)
	{
		json_parser::parse_global_status(json, sss);
		if(sss.size() > 0)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "get %d dlrs ", (int)sss.size());
		}
		count += (int)sss.size();
		print_dlrs(sss);
	}
	return count;
}
static int query_mos_simulator()
{
	std::vector<sm_mo> mos;
	std::string json;
	int count = 0;
	cb_client_get_mos(json);
	if(json.length() > 0)
	{
		json_parser::parse_mos(json, mos);
		count = (int)mos.size();
		if(count > 0)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "get %d mos ", count);
			s_total_mos += count;
			
			for(int i = 0; i < count; i++)
			{
				//logger_log(__FUNCTION__, LEVEL_DEBUG, "a mo, src:%s, content:%s ", mos.at(i).src_addr_.c_str(), mos.at(i).content_.c_str());
#ifdef DEBUG_MOS_ORDER
				static std::string last_src_addr = "";
				if(last_src_addr.length() == 0)
				{
					last_src_addr = mos.at(i).src_addr_;
				}
				else
				{
					
					boost::uint64_t src1 = datatype_convert<boost::uint64_t>(last_src_addr);
					boost::uint64_t src2 = datatype_convert<boost::uint64_t>(mos.at(i).src_addr_);
					if(src2 - src1 != 1)
					{
						logger_log(__FUNCTION__, LEVEL_WARNING, "mo disorder, %s VS %s ", last_src_addr.c_str(), mos.at(i).src_addr_.c_str());
					}
					last_src_addr = mos.at(i).src_addr_;
				}
#endif
			}
		}
	}
	
	return count;
}
static void generator(bool exit_if_submit_completed)
{
	unsigned int count = 0;
	int total_dlrs = 0;
	const int query_frequence = 3;
    while(!stop)
	{
		if(s_total_submitted <= s_total_to_submit)
		{
			s_total_submitted += generate_smis_simulator(1, "96688");
			boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
		}
		if(count % query_frequence == 0)
		{
			total_dlrs += query_dlrs_simulator();
			if((total_dlrs >= s_total_submitted) && (s_total_submitted >= s_total_to_submit) && exit_if_submit_completed)
			{
				logger_log(__FUNCTION__, LEVEL_WARNING, "submit %d and get dlrs successfully,total received %d mos, exit", total_dlrs, s_total_mos);
			}
			query_mos_simulator();
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
		}
		
		count++;
	}
}

void start_generator(const std::string& configer_file)
{
	read_auto_submit(configer_file);
	if(s_auto_submit && (s_total_to_submit > 0))
	{
		simulate_thread.reset(new boost::thread(&generator, false));
	}
}
void stop_generator()
{
	stop = true;
	if(simulate_thread.get())
	{
		simulate_thread->join();
	}
}
