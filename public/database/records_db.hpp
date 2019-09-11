/*
 *  records_db.hpp
 *  SMPPServer
 *
 *  Created by Zhu Nengjie on 5/25/15.
 *  Copyright 2015 Bifang technology. All rights reserved.
 *
 */
#ifndef RECORDS_DB_H
#define RECORDS_DB_H
#include "cb_protocol_public.hpp"
#include <boost/thread.hpp>
#ifdef USE_MYSQL
#include "mysql.h"
#endif

class records_db
{
public:
	records_db();
	~records_db();
	bool do_connect(const std::string& host_name, const std::string& user_name, const std::string& password, const std::string& db_name);
	
protected:
	boost::mutex db_mutex_;
#ifdef USE_MYSQL
	MYSQL* mysql_handle_;
#else
	void* mysql_handle_;
#define MYSQL_RES void
#endif
	bool initialize_tables();
	bool process_select(const std::string& query_string, MYSQL_RES** res_set);
	std::string dlrs_table_name_;
	std::string mos_table_name_;
	std::string generate_dlrs_table_name();
	std::string generate_mos_table_name();
protected:
	void close_auto_commit();
	void auto_commit(bool suc);
	bool peek_smis(const std::string& table_name, int to_peek, std::vector<cb_sm_information>& smis);
	bool delete_smis(const std::string& table_name, const std::vector<cb_sm_information>& smis);
	
public:
	bool get_smis(const std::string& table_name, int to_peek, std::vector<cb_sm_information>& smis);
	bool insert_smis(const std::vector<cb_sm_information>& smis);
	bool insert_mos(const std::vector<cb_sm_information>& smis);
};

#endif
