//
//  smppserver_runtime.cpp
//  SMPPServer
//
//  Created by Alex on 5/20/15.
//  Copyright (c) 2015 Bifang technology. All rights reserved.
//
#include "algorithm.hpp"
#include "smppserver_runtime.h"
#include "configer.hpp"
#include "logger.hpp"
#include <boost/thread.hpp>
#include "smpp_protocol_impl.hpp"
#include "smis_db.hpp"
#include "records_db.hpp"
#include "md5.h"
#include "vendor_runtime_demo.hpp"

smppserver_runtime* smppserver_runtime::instance_ = NULL;
boost::mutex smppserver_runtime::mutex_;

smppserver_statistic* smppserver_statistic::instance_ = NULL;
boost::mutex smppserver_statistic::mutex_;


boost::shared_ptr<vendor_runtime> vendor_runtime_;

smppserver_runtime::smppserver_runtime()
{
}
bool smppserver_runtime::read_configer()
{
	extern std::string get_config_file_path();
	std::string config_file = get_config_file_path();
	if(boost::filesystem::exists(config_file))
	{
		configer conf(config_file);
		//read server
		std::string sec_smpp_server = "smpp_server";
		std::string sec_db = "db";
		configer_.ip = conf.get_string(sec_smpp_server, "ip", "0.0.0.0");
		configer_.port = conf.get_string(sec_smpp_server, "port", "8800");
		configer_.need_authorize = (conf.get_int(sec_smpp_server, "need_auth", 1) == 1);
		configer_.use_hex_decimal_message_id = (conf.get_int(sec_smpp_server, "use_hex_decimal_message_id", 0) == 1);
		configer_.server_id = conf.get_string(sec_smpp_server, "server_id", "");
		if(configer_.server_id.length() == 0)
		{
			configer_.server_id = "SMPPSERVER " + format_decimal_to_string((unsigned int)time(NULL) - 100000000);
		}
		configer_.total_generated_mos = conf.get_int(sec_smpp_server, "total_generated_mos", 0);
		configer_.vendor_index = conf.get_int(sec_smpp_server, "vendor_index", 0);
		configer_.use_buffer_db = (conf.get_int(sec_smpp_server, "use_buffer_db", 0) == 1);
		configer_.setting_server_url = conf.get_string(sec_smpp_server, "setting_server_url", "");

		configer_.record_mt_mo = (conf.get_int(sec_smpp_server, "record_mt_mo", 0) == 1);
		
		configer_.smis_buffer_db_host_name = conf.get_string(sec_db, "smis_buffer_db_host_name", "");
		configer_.smis_buffer_db_user_name = conf.get_string(sec_db, "smis_buffer_db_user_name", "");
		configer_.smis_buffer_db_password = conf.get_string(sec_db, "smis_buffer_db_password", "");
		configer_.smis_buffer_db_db_name = conf.get_string(sec_db, "smis_buffer_db_db_name", "");
		if(configer_.use_buffer_db)
		{
			if(configer_.smis_buffer_db_host_name.length() == 0
			   || configer_.smis_buffer_db_user_name.length() == 0
			   || configer_.smis_buffer_db_password.length() == 0
			   || configer_.smis_buffer_db_db_name.length() == 0)
			{
				logger_log(__FUNCTION__, LEVEL_ERROR, "in db mode, smis buffer db information must be set correctly.");
				return false;
			}
		}

		
		configer_.records_db_host_name = conf.get_string(sec_db, "records_db_host_name", "");
		configer_.records_db_user_name = conf.get_string(sec_db, "records_db_user_name", "");
		configer_.records_db_password = conf.get_string(sec_db, "records_db_password", "");
		configer_.records_db_db_name = conf.get_string(sec_db, "records_db_db_name", "");
		if(configer_.record_mt_mo)
		{
			if(configer_.records_db_host_name.length() == 0
			   || configer_.records_db_user_name.length() == 0
			   || configer_.records_db_password.length() == 0
			   || configer_.records_db_db_name.length() == 0)
			{
				logger_log(__FUNCTION__, LEVEL_ERROR, "in want to record mt and mo, records db information must be set correctly.");
				return false;
			}
		}
		
		return true;
	}
	else
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "%s not exist", config_file.c_str());
		return true;
	}
}

bool smppserver_runtime::run_smppserver()
{
	try
	{
		if(!read_configer())
		{
			return false;
		}
		smppserver_datasource::get_instance()->set_configer(configer_);
		std::vector<listen_address> las;
		listen_address la;
		la.ip = configer_.ip;
		la.port = configer_.port;
		las.push_back(la);
		logger_log(__FUNCTION__, LEVEL_DEBUG, "smpp server %s starts at: %s:%s, vendor_index:%d, db mode:%d.", configer_.server_id.c_str(), configer_.ip.c_str(), configer_.port.c_str(), (int)configer_.vendor_index, (int)configer_.use_buffer_db);
		finished_ = false;
		if(configer_.vendor_index == 0)//使用系统自带的模拟网关处理
		{
			vendor_runtime_.reset(new vendor_runtime_demo());
		}
		else if(configer_.vendor_index == 1)
		{
			//
			//自己实现从vendor_runtime类继承的协议
			assert(false);
		}
		vendor_runtime_->run();
		unsigned int cores = boost::thread::hardware_concurrency();
		server_.reset(new server(las, cores));
		server_->run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return true;
}

void smppserver_runtime::stop_smppserver()
{
	finished_ = true;
	vendor_runtime_->stop();
	server_->finish();
}



