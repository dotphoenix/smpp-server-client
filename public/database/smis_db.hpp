/*
 *  smis_db.hpp
 *  SMPPServer
 *
 *  smis_db is used to be as a cache of all processes, such as smpp server, cmpp server, gageways manager
 *
 *  Created by Zhu Nengjie on 4/25/15.
 *  Copyright 2015 Bifang technology. All rights reserved.
 *
 */
#ifndef SMIS_DB_H
#define SMIS_DB_H
#ifdef USE_MYSQL
#include "mysql.h"
#endif
#include "cb_protocol_public.hpp"
#include <boost/thread.hpp>

class smis_db
{
public:
	smis_db();
	~smis_db();
	bool do_connect(const std::string& host_name, const std::string& user_name, const std::string& password, const std::string& db_name);
protected:
#ifdef USE_MYSQL
	MYSQL* mysql_handle_;
#else
	void* mysql_handle_;
	#define MYSQL_RES void
#endif
	
	boost::mutex db_mutex_;
	bool initialize_tables();
	bool process_select(const std::string& query_string, MYSQL_RES** res_set);
	
	void close_auto_commit();
	void auto_commit(bool suc);
	
public:
	bool peek_smis(int to_peek, std::vector<cb_sm_information>& smis);
	bool delete_smis(const std::vector<cb_sm_information>& smis);
	
	bool peek_dlrs(const std::string& server_id, const std::string& subaccount, int to_peek, std::vector<cb_sm_information>& smis);
	bool delete_dlrs(const std::vector<cb_sm_information>& smis);
	
	bool peek_mos(const std::string& server_id, const std::string& dst_addr, int to_peek, std::vector<cb_sm_information>& smis);
	bool delete_mos(const std::vector<cb_sm_information>& smis);
public:
	/*
	 这里有优化的空间，设置一个get的缓冲，一次多取一些数据
	 */
	bool insert_to_submit_smis(const std::vector<cb_sm_information>& smis);
	bool get_to_submit_smis(int to_get, std::vector<cb_sm_information>& smis);
	
	bool insert_to_delivery_dlrs(const std::vector<cb_sm_information>& smis);
	bool get_to_delivery_dlrs(const std::string& server_id, const std::string& subaccount, int to_get, std::vector<cb_sm_information>& smis);
	
	bool insert_to_delivery_mos(const std::vector<cb_sm_information>& smis);
	bool get_to_delivery_mos(const std::string& server_id, const std::string& dst_addr, int to_get, std::vector<cb_sm_information>& smis);
};

#endif
