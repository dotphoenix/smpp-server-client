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
#include "configer.hpp"
#include "cb_client_api.h"
#include "clients_manager.h"

extern void start_generator(const std::string& configer_file);

static std::string get_config_file_file()
{
	boost::filesystem::path rootpath = boost::filesystem::initial_path<boost::filesystem::path>();
	boost::filesystem::path configerpath = rootpath ;
	configerpath += boost::filesystem::path::preferred_separator;
#if defined _WIN32 && defined _DEBUG
	configerpath += "../";
#endif
	configerpath += "configer/configer.ini";
	return configerpath.string();
}

extern bool start_simple_webserver(const std::string& port);
extern void stop_simple_webserver();
static bool g_finish = false;
static void sig_int(int num)
{
    g_finish = true;
	logger_log(__FUNCTION__, LEVEL_WARNING, "program will stopped.");
	cb_client_stop();
    return;
}

/*
 submit:
 http://ip:port/get_submit?dst_addrs=188888888;1899999999&src_addr=990099&content=哈哈@哈哈
 http://ip:port/get_dlrs
 http://ip:port/get_mos
 http://ip:port/get_statistic
 */
//#define CHECK_VERSION
bool checkValid()
{
#ifdef CHECK_VERSION
	time_t tt;
	time(&tt);
	tm* t = localtime(&tt);
	int year = (unsigned int)t->tm_year + 1900;
	int month = (unsigned int)t->tm_mon + 1;
	int day =  (unsigned int)t->tm_mday;

	if(year > 2019 || (year == 2019 && month > 10) || (year == 2019 && month == 10 && day > 30))
	{
		logger_log(__FUNCTION__, LEVEL_ERROR, "Invalid version, please contact dotphoenix@qq.com for more information");
		return false;
	}
	return true;
#else
	return true;
#endif

}

void print_copyright(bool console)
{
	std::string version = "V1.2.0(Build.190909)";
	std::string copyright = "\n\nSmpp Client ";
	copyright = copyright + version ;
	copyright = copyright + "\nCopyright (C) 2019, dotphoenix@qq.com, All right reserved.\n";
	if(console)
	{
		std::cout<<copyright;
	}
	else
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "%s", copyright.c_str());
	}
}



int main(int argc, const char * argv[])
{
	signal(SIGINT, sig_int);
	print_copyright(true);
	std::string port = "8000";
	if (argc == 2)
	{
		port = argv[1];
	}

	cb_client_start(get_config_file_file(), true);
	print_copyright(false);
	start_generator(get_config_file_file());
	start_simple_webserver(port);
	if(!checkValid())
	{
		return 1;
	}
	while(!g_finish)
	{
		static int statistic_count = 0;
		
		if(statistic_count >= 300)
		{
			std::string s;
			cb_client_get_statistic_string(s);
			logger_log(__FUNCTION__, LEVEL_WARNING, "%s", s.c_str());
			statistic_count = 0;
		}
		#define AUTOMATICALLY_GET_DRLS_AND_MOS
#ifdef AUTOMATICALLY_GET_DRLS_AND_MOS
		static int query_dlr_mo_count = 0;
		if(query_dlr_mo_count >= 30) //30 seconds
		{
			std::string json;
			cb_client_get_mos(json);
			if(json.length() > 0)
			{
				logger_log(__FUNCTION__, LEVEL_WARNING, "automatically get mos: \n%s", json.c_str());
			}
			int n = 0;
			while((n++) < 100)
			{
				cb_client_get_dlrs(20, json);
				if(json.length() == 0)
				{
					break;
				}
				else
				{
					logger_log(__FUNCTION__, LEVEL_WARNING, "automatically get dlrs: \n%s", json.c_str());
				}
			}
			query_dlr_mo_count = 0;
		}
		if(!checkValid())
		{
			return 1;
		}
		query_dlr_mo_count++;
#endif
		statistic_count++;
		
		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	}
	cb_client_stop();
	stop_simple_webserver();
	return 0;
}
