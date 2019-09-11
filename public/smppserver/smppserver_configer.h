//
//  smppserver_config.h
//  SMPPServer
//
//  Created by Alex on 2018/8/24.
//  Copyright © 2018 BiFang technology. All rights reserved.
//

#ifndef smppserver_config_h
#define smppserver_config_h
#include "algorithm.hpp"

struct smppserver_configer
{
	std::string ip;
	std::string port;
	bool need_authorize;
	bool use_hex_decimal_message_id;
	std::string server_id;
	int total_generated_mos;
	int vendor_index; //0表示本地模拟网关，自动产生数据, 1表示使用vendor_runtime_1, 以此类推
	bool use_buffer_db;
	bool record_mt_mo;
	std::string setting_server_url;
	//smis buffer db
	std::string smis_buffer_db_host_name;
	std::string smis_buffer_db_user_name;
	std::string smis_buffer_db_password;
	std::string smis_buffer_db_db_name;
	
	//records
	std::string records_db_host_name;
	std::string records_db_user_name;
	std::string records_db_password;
	std::string records_db_db_name;
	
	smppserver_configer()
	{
		ip = "0.0.0.0";
		port = "8800";
		need_authorize = true;
		use_hex_decimal_message_id = false;
		server_id = "SMPPSERVER" + format_decimal_to_string((unsigned int)time(NULL) - 100000000);
		total_generated_mos = 0;
		vendor_index = 1; //如果用于模拟网关，设为0
		use_buffer_db = false;
		record_mt_mo = false;
		setting_server_url = "http://192.168.1.33:8080/settingserver/jsonservlet";
		
	}
};

#endif /* smppserver_config_h */
