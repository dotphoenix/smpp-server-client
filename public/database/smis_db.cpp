/*
 *  smis_db.cpp
 *  SMPPServer
 *
 *  Created by Zhu Nengjie on 4/25/15.
 *  Copyright 2015 Bifang technology. All rights reserved.
 *
 */
#include "algorithm.hpp"
#include "smis_db.hpp"
#include "db_strings.hpp"
#include "md5.h"
#include <time.h>

#ifdef USE_MYSQL

#define PRINT_ERROR(fun) \
logger_log(__FUNCTION__, LEVEL_WARNING, "call %s failed, error is %u (%s)", fun, mysql_errno(mysql_handle_), mysql_error(mysql_handle_));

smis_db::smis_db()
{
	mysql_handle_ = mysql_init (NULL); /* allocate, initialize connection handler */
	if (mysql_handle_ == NULL)
	{
		logger_log(__FUNCTION__, LEVEL_ERROR, "mysql_init() failed");
	}
}
smis_db::~smis_db()
{
	if (mysql_handle_ != NULL)
	{
		mysql_close(mysql_handle_);
		mysql_handle_ = NULL;
	}
}

bool smis_db::initialize_tables()
{
	std::string query_string = db_strings::create + db_strings::table + db_strings::if_not_exists
	+ db_strings::to_submit_smis_table + db_strings::left_parenthesis
	+ db_strings::server_id + db_strings::varchar_32
	+ db_strings::comma + db_strings::subaccount + db_strings::varchar_32
	+ db_strings::comma + db_strings::message_id_of_protocol_server + db_strings::varchar_32
	+ db_strings::comma + db_strings::sn_by_client + db_strings::uint32
	+ db_strings::comma + db_strings::dst_addr + db_strings::varchar_32
	+ db_strings::comma + db_strings::ip + db_strings::varchar_16
	+ db_strings::comma + db_strings::src_addr + db_strings::varchar_32
	+ db_strings::comma + db_strings::submit_time_of_client + db_strings::uint64
	+ db_strings::comma + db_strings::udhi_reference + db_strings::uint8
	+ db_strings::comma + db_strings::udhi_total_parts + db_strings::uint8
	+ db_strings::comma + db_strings::udhi_part_index + db_strings::uint8
	+ db_strings::comma + db_strings::datacoding_type + db_strings::uint8
	+ db_strings::comma + db_strings::content + db_strings::varchar_256
	+ db_strings::comma + db_strings::content_hash + db_strings::varchar_64
	+ db_strings::comma + db_strings::protocol_of_client + db_strings::varchar_8
	+ db_strings::right_parenthesis + db_strings::semicolon;
	if (mysql_query (mysql_handle_, query_string.c_str()) != 0)
	{
		PRINT_ERROR("mysql_query");
		return false;
	}
	query_string = db_strings::create + db_strings::table + db_strings::if_not_exists
	+ db_strings::to_delivery_dlrs_table + db_strings::left_parenthesis
	+ db_strings::server_id + db_strings::varchar_32
	+ db_strings::comma + db_strings::subaccount + db_strings::varchar_32
	+ db_strings::comma + db_strings::message_id_of_protocol_server + db_strings::varchar_32
	+ db_strings::comma + db_strings::sn_by_protocol_server + db_strings::uint32
	+ db_strings::comma + db_strings::error_status + db_strings::varchar_16
	+ db_strings::comma + db_strings::error_code + db_strings::uint32
	+ db_strings::right_parenthesis + db_strings::semicolon;
	if (mysql_query (mysql_handle_, query_string.c_str()) != 0)
	{
		PRINT_ERROR("mysql_query");
		return false;
	}
	
	query_string = db_strings::create + db_strings::table + db_strings::if_not_exists
	+ db_strings::to_delivery_mos_table + db_strings::left_parenthesis
	+ db_strings::server_id + db_strings::varchar_32
	+ db_strings::comma + db_strings::subaccount + db_strings::varchar_32
	+ db_strings::comma + db_strings::sn_by_protocol_server + db_strings::uint32
	+ db_strings::comma + db_strings::src_addr + db_strings::varchar_32
	+ db_strings::comma + db_strings::dst_addr + db_strings::varchar_32
	+ db_strings::comma + db_strings::content + db_strings::varchar_256
	+ db_strings::right_parenthesis + db_strings::semicolon;
	if (mysql_query (mysql_handle_, query_string.c_str()) != 0)
	{
		PRINT_ERROR("mysql_query");
		return false;
	}
	return true;
}

bool smis_db::do_connect(const std::string& host_name, const std::string& user_name, const std::string& password,const std::string& db_name)
{
	if(mysql_handle_ == NULL)
	{
		return false;
	}
	if (mysql_real_connect (mysql_handle_, host_name.c_str(), user_name.c_str(), password.c_str(), db_name.c_str(), 0, NULL, 0) == NULL)
	{
		PRINT_ERROR("mysql_real_connect")
		return false;;
	}
	initialize_tables();
	return true;
}
//do not forget to call mysql_free_result to free *res_set
bool smis_db::process_select(const std::string& query_string, MYSQL_RES** res_set)
{
	if (mysql_query(mysql_handle_, query_string.c_str()) != 0) // the query failed
	{
		PRINT_ERROR("mysql_query")
		return false;
	}
	// the query succeeded; determine whether or not it returns data
	*res_set = mysql_store_result(mysql_handle_);
	if (*res_set == NULL)
	{
		//does the lack of occurred or that no result set was returned
		//a result set mean that an error no result set was returned?
		if (mysql_field_count(mysql_handle_) > 0)
		{
			//a result set was expected, but mysql_store_result() did not return one;
			//this means an error occurred
			PRINT_ERROR("problem processing result set ");
			return false;
		}
		else
		{
			//no result set was returned; query returned no data (it was not a SELECT, SHOW, DESCRIBE, or EXPLAIN),
			//so just report number of rows affected by query */
			logger_log(__FUNCTION__, LEVEL_DEBUG, "no result set returns, only %d rows are affected", (int)mysql_affected_rows(mysql_handle_));
			return false;
		}
	}
	else // a result set was returned
	{
		// the caller should process the rows, and remember to free the result set
		return true;
	}
}
void smis_db::close_auto_commit()
{
	if(mysql_query(mysql_handle_, "START TRANSACTION") != 0)
	{
		PRINT_ERROR("mysql_query");
	}
}
void smis_db::auto_commit(bool suc)
{
	int ret = 0;
	if(suc)
	{
		ret = mysql_query(mysql_handle_, "COMMIT");
	}
	else
	{
		ret = mysql_query(mysql_handle_, "ROLLBACK");
	}
	if(ret != 0)
	{
		PRINT_ERROR("mysql_query");
	}
}
bool smis_db::insert_to_submit_smis(const std::vector<cb_sm_information> &smis)
{
	boost::mutex::scoped_lock lock(db_mutex_);
	close_auto_commit();
	for(size_t i = 0; i < smis.size(); i++)
	{
		const cb_sm_information& smi = smis.at(i);
		std::string insert_string =  db_strings::insert_into
		+ db_strings::to_submit_smis_table + db_strings::left_parenthesis
		+ db_strings::server_id
		+ db_strings::comma + db_strings::subaccount
		+ db_strings::comma + db_strings::message_id_of_protocol_server
		+ db_strings::comma + db_strings::sn_by_client
		+ db_strings::comma + db_strings::dst_addr
		+ db_strings::comma + db_strings::ip
		+ db_strings::comma + db_strings::src_addr
		+ db_strings::comma + db_strings::submit_time_of_client
		+ db_strings::comma + db_strings::udhi_reference
		+ db_strings::comma + db_strings::udhi_total_parts
		+ db_strings::comma + db_strings::udhi_part_index
		+ db_strings::comma + db_strings::datacoding_type
		+ db_strings::comma + db_strings::content
		+ db_strings::comma + db_strings::content_hash
		+ db_strings::comma + db_strings::protocol_of_client
		+ db_strings::right_parenthesis
		+ db_strings::values + db_strings::left_parenthesis
		+ db_strings::escape_char + smi.server_id + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.subaccount + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.message_id_of_protocol_server + db_strings::escape_char
		+ db_strings::comma + datatype_convert<std::string>(smi.sn_by_client)
		+ db_strings::comma + db_strings::escape_char + smi.dst_addr + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.ip + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.src_addr + db_strings::escape_char
		+ db_strings::comma + datatype_convert<std::string>(smi.submit_time_of_client)
		+ db_strings::comma + datatype_convert<std::string>((int)smi.udhi_reference)
		+ db_strings::comma + datatype_convert<std::string>((int)smi.udhi_total_parts)
		+ db_strings::comma + datatype_convert<std::string>((int)smi.udhi_part_index)
		+ db_strings::comma + datatype_convert<std::string>((int)smi.datacoding_type)
		+ db_strings::comma + db_strings::escape_char + smi.content + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.content_hash + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.protocol_of_client + db_strings::escape_char
		+ db_strings::right_parenthesis + db_strings::semicolon;
		
		if (mysql_query (mysql_handle_, insert_string.c_str()) != 0)
		{
			PRINT_ERROR("mysql_query");
			auto_commit(false);
			return false;
		}
	}
	auto_commit(true);
	return true;
}

bool smis_db::peek_smis(int to_peek, std::vector<cb_sm_information>& smis)
{
	std::string select_string =  db_strings::select + db_strings::star_char + db_strings::from
	+ db_strings::to_submit_smis_table
	+ db_strings::limit + datatype_convert<std::string>(to_peek)
	+ db_strings::semicolon;
	MYSQL_RES* res_set = NULL;
	bool ret = process_select(select_string, &res_set);
	if(!ret)
	{
		return false;
	}
	int rows = (int)mysql_num_rows(res_set);
	if(rows == 0)
	{
		mysql_free_result(res_set);
		return true;
	}
	MYSQL_ROW row;
	while((row = mysql_fetch_row(res_set)) != NULL)
	{
		if(mysql_num_fields(res_set) == 15)
		{
			cb_sm_information smi;
			smi.server_id = row[0];
			smi.subaccount = row[1];
			smi.message_id_of_protocol_server = row[2];
			smi.sn_by_client = datatype_convert<unsigned int>(row[3]);
			smi.dst_addr = row[4];
			smi.ip = row[5];
			smi.src_addr = row[6];
			smi.submit_time_of_client = datatype_convert<boost::uint64_t>(row[7]);
			smi.udhi_reference = (unsigned char)datatype_convert<int>(row[8]);
			smi.udhi_total_parts = (unsigned char)datatype_convert<int>(row[9]);
			smi.udhi_part_index = (unsigned char)datatype_convert<int>(row[10]);
			smi.datacoding_type = (unsigned char)datatype_convert<int>(row[11]);
			smi.content = row[12];
			smi.content_hash = row[13];
			smi.protocol_of_client = row[14];
			smis.push_back(smi);
		}
	}
	mysql_free_result(res_set);
	return true;
}
bool smis_db::delete_smis(const std::vector<cb_sm_information>& smis)
{
	close_auto_commit();
	for(size_t i = 0; i < smis.size(); i++)
	{
		const cb_sm_information& smi = smis.at(i);
		std::string delete_string =  db_strings::delete_from
		+ db_strings::to_submit_smis_table
		+ db_strings::where_clause
		+ db_strings::server_id + db_strings::equal + db_strings::escape_char + smi.server_id + db_strings::escape_char
		+ db_strings::and_operator
		+ db_strings::subaccount + db_strings::equal + db_strings::escape_char + smi.subaccount + db_strings::escape_char
		+ db_strings::and_operator
		+ db_strings::sn_by_client + db_strings::equal + datatype_convert<std::string>(smi.sn_by_client)
		+ db_strings::and_operator
		+ db_strings::submit_time_of_client + db_strings::equal + datatype_convert<std::string>(smi.submit_time_of_client)
		+ db_strings::semicolon;
		
		if (mysql_query (mysql_handle_, delete_string.c_str()) != 0)
		{
			PRINT_ERROR("mysql_query");
			auto_commit(false);
			return false;
		}
	}
	auto_commit(true);
	return true;
	
}
bool smis_db::get_to_submit_smis(int to_get, std::vector<cb_sm_information>& smis)
{
	boost::mutex::scoped_lock lock(db_mutex_);
	peek_smis(to_get, smis);
	if(!smis.empty())
	{
		delete_smis(smis);
		return  true;
	}
	return false;
}

bool smis_db::insert_to_delivery_dlrs(const std::vector<cb_sm_information> &smis)
{
	boost::mutex::scoped_lock lock(db_mutex_);
	close_auto_commit();
	for(size_t i = 0; i < smis.size(); i++)
	{
		const cb_sm_information& smi = smis.at(i);
		std::string insert_string =  db_strings::insert_into
		+ db_strings::to_delivery_dlrs_table + db_strings::left_parenthesis
		+ db_strings::server_id
		+ db_strings::comma + db_strings::subaccount
		+ db_strings::comma + db_strings::message_id_of_protocol_server
		+ db_strings::comma + db_strings::sn_by_protocol_server
		+ db_strings::comma + db_strings::error_status
		+ db_strings::comma + db_strings::error_code
		+ db_strings::right_parenthesis
		+ db_strings::values + db_strings::left_parenthesis
		+ db_strings::escape_char + smi.server_id + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.subaccount + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.message_id_of_protocol_server + db_strings::escape_char
		+ db_strings::comma + datatype_convert<std::string>(smi.sn_by_protocol_server)
		+ db_strings::comma + db_strings::escape_char + smi.error_status + db_strings::escape_char
		+ db_strings::comma + datatype_convert<std::string>(smi.error_code)
		+ db_strings::right_parenthesis + db_strings::semicolon;
		
		if (mysql_query (mysql_handle_, insert_string.c_str()) != 0)
		{
			PRINT_ERROR("mysql_query");
			auto_commit(false);
			return false;
		}
	}
	auto_commit(true);
	return true;
}

bool smis_db::peek_dlrs(const std::string& server_id, const std::string& subaccount, int to_peek, std::vector<cb_sm_information>& smis)
{
	std::string select_string =  db_strings::select + db_strings::star_char + db_strings::from
	+ db_strings::to_delivery_dlrs_table
	+ db_strings::where_clause
	+ db_strings::server_id + db_strings::equal +  db_strings::escape_char + server_id +  db_strings::escape_char
	+ db_strings::and_operator
	+ db_strings::subaccount + db_strings::equal +  db_strings::escape_char + subaccount +  db_strings::escape_char
	+ db_strings::limit + datatype_convert<std::string>(to_peek)
	+ db_strings::semicolon;
	MYSQL_RES* res_set = NULL;
	bool ret = process_select(select_string, &res_set);
	if(!ret)
	{
		return false;
	}
	int rows = (int)mysql_num_rows(res_set);
	if(rows == 0)
	{
		mysql_free_result(res_set);
		return true;
	}
	MYSQL_ROW row;
	while((row = mysql_fetch_row(res_set)) != NULL)
	{
		if(mysql_num_fields(res_set) == 6)
		{
			cb_sm_information smi;
			smi.server_id = row[0];
			smi.subaccount = row[1];
			smi.message_id_of_protocol_server = row[2];
			smi.sn_by_protocol_server = datatype_convert<unsigned int>(row[3]);
			smi.error_status = row[4];
			smi.error_code = datatype_convert<unsigned int>(row[5]);
			smis.push_back(smi);
		}
	}
	mysql_free_result(res_set);
	return true;
}
bool smis_db::delete_dlrs(const std::vector<cb_sm_information>& smis)
{
	close_auto_commit();
	for(size_t i = 0; i < smis.size(); i++)
	{
		const cb_sm_information& smi = smis.at(i);
		std::string delete_string =  db_strings::delete_from
		+ db_strings::to_delivery_dlrs_table
		+ db_strings::where_clause
		+ db_strings::server_id + db_strings::equal + db_strings::escape_char + smi.server_id + db_strings::escape_char
		+ db_strings::and_operator
		+ db_strings::subaccount + db_strings::equal + db_strings::escape_char + smi.subaccount + db_strings::escape_char
		+ db_strings::and_operator
		+ db_strings::message_id_of_protocol_server + db_strings::equal + db_strings::escape_char + smi.message_id_of_protocol_server + db_strings::escape_char
		+ db_strings::semicolon;
		
		if (mysql_query (mysql_handle_, delete_string.c_str()) != 0)
		{
			PRINT_ERROR("mysql_query");
			auto_commit(false);
			return false;
		}
	}
	auto_commit(true);
	return true;
	
}
bool smis_db::get_to_delivery_dlrs(const std::string& server_id, const std::string& subaccount, int to_get, std::vector<cb_sm_information>& smis)
{
	boost::mutex::scoped_lock lock(db_mutex_);
	peek_dlrs(server_id, subaccount, to_get, smis);
	if(!smis.empty())
	{
		delete_dlrs(smis);
		return true;
	}
	return false;
}

bool smis_db::insert_to_delivery_mos(const std::vector<cb_sm_information> &smis)
{
	boost::mutex::scoped_lock lock(db_mutex_);
	close_auto_commit();
	for(size_t i = 0; i < smis.size(); i++)
	{
		const cb_sm_information& smi = smis.at(i);
		std::string insert_string =  db_strings::insert_into
		+ db_strings::to_delivery_mos_table + db_strings::left_parenthesis
		+ db_strings::server_id
		+ db_strings::comma + db_strings::subaccount
		+ db_strings::comma + db_strings::sn_by_protocol_server
		+ db_strings::comma + db_strings::src_addr
		+ db_strings::comma + db_strings::dst_addr
		+ db_strings::comma + db_strings::content
		+ db_strings::right_parenthesis
		+ db_strings::values + db_strings::left_parenthesis
		+ db_strings::escape_char + smi.server_id + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.subaccount + db_strings::escape_char
		+ db_strings::comma + datatype_convert<std::string>(smi.sn_by_protocol_server)
		+ db_strings::comma + db_strings::escape_char + smi.src_addr + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.dst_addr + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.content + db_strings::escape_char
		+ db_strings::right_parenthesis + db_strings::semicolon;
		
		if (mysql_query (mysql_handle_, insert_string.c_str()) != 0)
		{
			PRINT_ERROR("mysql_query");
			auto_commit(false);
			return false;
		}
	}
	auto_commit(true);
	return true;
}

bool smis_db::peek_mos(const std::string& server_id, const std::string& dst_addr, int to_peek, std::vector<cb_sm_information>& smis)
{
	std::string select_string =  db_strings::select + db_strings::star_char + db_strings::from
	+ db_strings::to_delivery_mos_table
	+ db_strings::where_clause
	+ db_strings::server_id + db_strings::equal +  db_strings::escape_char + server_id +  db_strings::escape_char
	+ db_strings::and_operator
	+ db_strings::dst_addr + db_strings::equal +  db_strings::escape_char + dst_addr +  db_strings::escape_char
	+ db_strings::limit + datatype_convert<std::string>(to_peek)
	+ db_strings::semicolon;
	MYSQL_RES* res_set = NULL;
	bool ret = process_select(select_string, &res_set);
	if(!ret)
	{
		return false;
	}
	int rows = (int)mysql_num_rows(res_set);
	if(rows == 0)
	{
		mysql_free_result(res_set);
		return true;
	}
	MYSQL_ROW row;
	while((row = mysql_fetch_row(res_set)) != NULL)
	{
		if(mysql_num_fields(res_set) == 6)
		{
			cb_sm_information smi;
			smi.server_id = row[0];
			smi.subaccount = row[1];
			smi.sn_by_protocol_server = datatype_convert<unsigned int>(row[2]);
			smi.src_addr = row[3];
			smi.dst_addr = row[4];
			smi.content = row[5];
			smis.push_back(smi);
		}
	}
	mysql_free_result(res_set);
	return true;
}
bool smis_db::delete_mos(const std::vector<cb_sm_information>& smis)
{
	close_auto_commit();
	for(size_t i = 0; i < smis.size(); i++)
	{
		const cb_sm_information& smi = smis.at(i);
		std::string delete_string =  db_strings::delete_from
		+ db_strings::to_delivery_mos_table
		+ db_strings::where_clause
		+ db_strings::server_id + db_strings::equal + db_strings::escape_char + smi.server_id + db_strings::escape_char
		+ db_strings::and_operator
		+ db_strings::src_addr + db_strings::equal + db_strings::escape_char + smi.src_addr + db_strings::escape_char
		+ db_strings::and_operator
		+ db_strings::dst_addr + db_strings::equal + db_strings::escape_char + smi.dst_addr + db_strings::escape_char
		+ db_strings::and_operator
		+ db_strings::content_hash + db_strings::equal + db_strings::escape_char + smi.content_hash + db_strings::escape_char
		+ db_strings::semicolon;
		
		if (mysql_query (mysql_handle_, delete_string.c_str()) != 0)
		{
			PRINT_ERROR("mysql_query");
			auto_commit(false);
			return false;
		}
	}
	auto_commit(true);
	return true;
	
}
bool smis_db::get_to_delivery_mos(const std::string& server_id, const std::string& dst_addr, int to_get, std::vector<cb_sm_information>& smis)
{
	boost::mutex::scoped_lock lock(db_mutex_);
	peek_mos(server_id, dst_addr, to_get, smis);
	if(!smis.empty())
	{
		delete_mos(smis);
		return true;
	}
	return false;
}
#else

smis_db::smis_db()
{
}
smis_db::~smis_db()
{
}

bool smis_db::initialize_tables()
{
	return true;
}

bool smis_db::do_connect(const std::string& host_name, const std::string& user_name, const std::string& password,const std::string& db_name)
{
	return true;
}
//do not forget to call mysql_free_result to free *res_set
bool smis_db::process_select(const std::string& query_string, MYSQL_RES** res_set)
{
	return false;
}
void smis_db::close_auto_commit()
{
}
void smis_db::auto_commit(bool suc)
{
}
bool smis_db::insert_to_submit_smis(const std::vector<cb_sm_information> &smis)
{
	return true;
}

bool smis_db::peek_smis(int to_peek, std::vector<cb_sm_information>& smis)
{
	return true;
}
bool smis_db::delete_smis(const std::vector<cb_sm_information>& smis)
{
	return true;
}
bool smis_db::get_to_submit_smis(int to_get, std::vector<cb_sm_information>& smis)
{
	return false;
}

bool smis_db::insert_to_delivery_dlrs(const std::vector<cb_sm_information> &smis)
{
	return true;
}

bool smis_db::peek_dlrs(const std::string& server_id, const std::string& subaccount, int to_peek, std::vector<cb_sm_information>& smis)
{
	return true;
}
bool smis_db::delete_dlrs(const std::vector<cb_sm_information>& smis)
{
	return true;
}
bool smis_db::get_to_delivery_dlrs(const std::string& server_id, const std::string& subaccount, int to_get, std::vector<cb_sm_information>& smis)
{
	return false;
}

bool smis_db::insert_to_delivery_mos(const std::vector<cb_sm_information> &smis)
{
	return true;
}

bool smis_db::peek_mos(const std::string& server_id, const std::string& dst_addr, int to_peek, std::vector<cb_sm_information>& smis)
{
	return true;
}
bool smis_db::delete_mos(const std::vector<cb_sm_information>& smis)
{
	return true;
	
}
bool smis_db::get_to_delivery_mos(const std::string& server_id, const std::string& dst_addr, int to_get, std::vector<cb_sm_information>& smis)
{
	return false;
}

#endif
