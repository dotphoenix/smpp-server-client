/*
 *  records_db.cpp
 *  SMPPServer
 *
 *  Created by Zhu Nengjie on 5/25/15.
 *  Copyright 2015 Bifang technology. All rights reserved.
 *
 */
#include "algorithm.hpp"
#include "records_db.hpp"
#include "db_strings.hpp"
#include "md5.h"
#include <time.h>
#ifdef USE_MYSQL

#define PRINT_ERROR(fun) \
logger_log(__FUNCTION__, LEVEL_WARNING, "call %s failed, error is %u (%s)", fun, mysql_errno(mysql_handle_), mysql_error(mysql_handle_));

records_db::records_db()
{
	mysql_handle_ = mysql_init (NULL); /* allocate, initialize connection handler */
	if (mysql_handle_ == NULL)
	{
		logger_log(__FUNCTION__, LEVEL_ERROR, "mysql_init() failed");
	}
}
records_db::~records_db()
{
	if (mysql_handle_ != NULL)
	{
		mysql_close(mysql_handle_);
		mysql_handle_ = NULL;
	}
}
std::string records_db::generate_dlrs_table_name()
{
	time_t tt = time(NULL);
	struct tm* t = localtime(&tt);
	return std::string("dlrs") + format_uint32_to_lenght(t->tm_year + 1900, 2) +
	format_uint32_to_lenght(t->tm_mon + 1, 2) + format_uint32_to_lenght(t->tm_mday, 2);
}
std::string records_db::generate_mos_table_name()
{
	time_t tt = time(NULL);
	struct tm* t = localtime(&tt);
	return std::string("mos") + format_uint32_to_lenght(t->tm_year + 1900, 2) +
	format_uint32_to_lenght(t->tm_mon + 1, 2);
}
bool records_db::initialize_tables()
{
	std::string table_name = generate_dlrs_table_name();
	if(table_name.compare(dlrs_table_name_) != 0)
	{
		dlrs_table_name_ = table_name;
		std::string query_string = db_strings::create + db_strings::table + db_strings::if_not_exists
		+ dlrs_table_name_ + db_strings::left_parenthesis
		+ db_strings::server_id + db_strings::varchar_32
		+ db_strings::comma + db_strings::company_account + db_strings::varchar_32
		+ db_strings::comma + db_strings::subaccount + db_strings::varchar_32
		+ db_strings::comma + db_strings::state + db_strings::uint8
		+ db_strings::comma + db_strings::sn_by_client + db_strings::uint32
		+ db_strings::comma + db_strings::sn_by_protocol_server + db_strings::uint32
		+ db_strings::comma + db_strings::sn_by_gateway + db_strings::uint32
		+ db_strings::comma + db_strings::sn_by_provider + db_strings::uint32
		+ db_strings::comma + db_strings::message_id_of_protocol_server + db_strings::varchar_32
		+ db_strings::comma + db_strings::message_id_of_provider + db_strings::varchar_32
		+ db_strings::comma + db_strings::dst_addr + db_strings::varchar_32
		+ db_strings::comma + db_strings::ip + db_strings::varchar_16
		+ db_strings::comma + db_strings::session_id + db_strings::varchar_32
		+ db_strings::comma + db_strings::gateway_id + db_strings::varchar_32
		+ db_strings::comma + db_strings::src_addr + db_strings::varchar_32
		+ db_strings::comma + db_strings::ack_status + db_strings::uint8
		+ db_strings::comma + db_strings::submit_time_of_client + db_strings::uint64
		+ db_strings::comma + db_strings::delivering_time_of_client + db_strings::uint64
		+ db_strings::comma + db_strings::delivery_time_of_client + db_strings::uint64
		+ db_strings::comma + db_strings::submit_time_of_gateway + db_strings::uint64
		+ db_strings::comma + db_strings::delivering_time_of_gateway + db_strings::uint64
		+ db_strings::comma + db_strings::delivery_time_of_gateway + db_strings::uint64
		+ db_strings::comma + db_strings::error_status + db_strings::varchar_16
		+ db_strings::comma + db_strings::error_code + db_strings::uint32
		+ db_strings::comma + db_strings::udhi_reference + db_strings::uint8
		+ db_strings::comma + db_strings::udhi_total_parts + db_strings::uint8
		+ db_strings::comma + db_strings::udhi_part_index + db_strings::uint8
		+ db_strings::comma + db_strings::datacoding_type + db_strings::uint8
		+ db_strings::comma + db_strings::content + db_strings::varchar_256
		+ db_strings::comma + db_strings::content_hash + db_strings::varchar_64
		+ db_strings::comma + db_strings::charge_mode_of_client + db_strings::uint8
		+ db_strings::comma + db_strings::price_of_client + db_strings::double_type
		+ db_strings::comma + db_strings::charge_mode_of_gateway + db_strings::uint8
		+ db_strings::comma + db_strings::price_of_gateway + db_strings::double_type
		+ db_strings::comma + db_strings::mcc + db_strings::varchar_8
		+ db_strings::comma + db_strings::mnc + db_strings::varchar_8
		+ db_strings::comma + db_strings::classification + db_strings::uint8
		+ db_strings::comma + db_strings::valid + db_strings::uint8
		+ db_strings::comma + db_strings::protocol_of_client + db_strings::varchar_8
		+ db_strings::comma + db_strings::protocol_of_gateway + db_strings::varchar_8
		+ db_strings::right_parenthesis + db_strings::semicolon;
		if (mysql_query (mysql_handle_, query_string.c_str()) != 0)
		{
			PRINT_ERROR("mysql_query");
			return false;
		}
	}
	
	table_name = generate_mos_table_name();
	if(table_name.compare(mos_table_name_) != 0)
	{
		mos_table_name_ = table_name;
		std::string query_string = db_strings::create + db_strings::table + db_strings::if_not_exists
		+ mos_table_name_ + db_strings::left_parenthesis
		+ db_strings::server_id + db_strings::varchar_32
		+ db_strings::comma + db_strings::company_account + db_strings::varchar_32
		+ db_strings::comma + db_strings::subaccount + db_strings::varchar_32
		+ db_strings::comma + db_strings::src_addr + db_strings::varchar_32
		+ db_strings::comma + db_strings::dst_addr + db_strings::varchar_32
		+ db_strings::comma + db_strings::sn_by_protocol_server + db_strings::uint32
		+ db_strings::comma + db_strings::sn_by_provider + db_strings::uint32
		+ db_strings::comma + db_strings::delivery_time_of_gateway + db_strings::uint64
		+ db_strings::comma + db_strings::delivery_time_of_client + db_strings::uint64
		+ db_strings::comma + db_strings::udhi_reference + db_strings::uint8
		+ db_strings::comma + db_strings::udhi_total_parts + db_strings::uint8
		+ db_strings::comma + db_strings::udhi_part_index + db_strings::uint8
		+ db_strings::comma + db_strings::datacoding_type + db_strings::uint8
		+ db_strings::comma + db_strings::content + db_strings::varchar_256
		+ db_strings::comma + db_strings::content_hash + db_strings::varchar_64
		+ db_strings::comma + db_strings::protocol_of_gateway + db_strings::varchar_8
		+ db_strings::right_parenthesis + db_strings::semicolon;
		if (mysql_query (mysql_handle_, query_string.c_str()) != 0)
		{
			PRINT_ERROR("mysql_query");
			return false;
		}
	}
	return true;
}

bool records_db::do_connect(const std::string& host_name, const std::string& user_name, const std::string& password,const std::string& db_name)
{
	if(mysql_handle_ == NULL)
	{
		return false;
	}
	if (mysql_real_connect (mysql_handle_, host_name.c_str(), user_name.c_str(), password.c_str(), db_name.c_str(), 0, NULL, 0) == NULL)
	{
		PRINT_ERROR("mysql_real_connect")
		return false;
	}
	initialize_tables();
	return true;
}
//do not forget to call mysql_free_result to free *res_set
bool records_db::process_select(const std::string& query_string, MYSQL_RES** res_set)
{
	boost::mutex::scoped_lock lock(db_mutex_);
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
			PRINT_ERROR("Problem processing result set ");
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
void records_db::close_auto_commit()
{
	if(mysql_query(mysql_handle_, "START TRANSACTION") != 0)
	{
		PRINT_ERROR("mysql_query");
	}
}
void records_db::auto_commit(bool suc)
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
bool records_db::insert_smis(const std::vector<cb_sm_information> &smis)
{
	boost::mutex::scoped_lock lock(db_mutex_);
	{
		initialize_tables();
	}
	close_auto_commit();
	for(size_t i = 0; i < smis.size(); i++)
	{
		const cb_sm_information& smi = smis.at(i);
		std::string insert_string =  db_strings::insert_into
		+ dlrs_table_name_ + db_strings::left_parenthesis
		+ db_strings::server_id
		+ db_strings::comma + db_strings::company_account
		+ db_strings::comma + db_strings::subaccount
		+ db_strings::comma + db_strings::state
		+ db_strings::comma + db_strings::sn_by_client
		+ db_strings::comma + db_strings::sn_by_protocol_server
		+ db_strings::comma + db_strings::sn_by_gateway
		+ db_strings::comma + db_strings::sn_by_provider
		+ db_strings::comma + db_strings::message_id_of_protocol_server
		+ db_strings::comma + db_strings::message_id_of_provider
		+ db_strings::comma + db_strings::dst_addr
		+ db_strings::comma + db_strings::ip
		+ db_strings::comma + db_strings::session_id
		+ db_strings::comma + db_strings::gateway_id
		+ db_strings::comma + db_strings::src_addr
		+ db_strings::comma + db_strings::ack_status
		+ db_strings::comma + db_strings::submit_time_of_client
		+ db_strings::comma + db_strings::delivering_time_of_client
		+ db_strings::comma + db_strings::delivery_time_of_client
		+ db_strings::comma + db_strings::submit_time_of_gateway
		+ db_strings::comma + db_strings::delivering_time_of_gateway
		+ db_strings::comma + db_strings::delivery_time_of_gateway
		+ db_strings::comma + db_strings::error_status
		+ db_strings::comma + db_strings::error_code
		+ db_strings::comma + db_strings::udhi_reference
		+ db_strings::comma + db_strings::udhi_total_parts
		+ db_strings::comma + db_strings::udhi_part_index
		+ db_strings::comma + db_strings::datacoding_type
		+ db_strings::comma + db_strings::content
		+ db_strings::comma + db_strings::content_hash
		+ db_strings::comma + db_strings::charge_mode_of_client
		+ db_strings::comma + db_strings::price_of_client
		+ db_strings::comma + db_strings::charge_mode_of_gateway
		+ db_strings::comma + db_strings::price_of_gateway
		+ db_strings::comma + db_strings::mnc
		+ db_strings::comma + db_strings::mcc
		+ db_strings::comma + db_strings::classification
		+ db_strings::comma + db_strings::valid
		+ db_strings::comma + db_strings::protocol_of_client
		+ db_strings::comma + db_strings::protocol_of_gateway
		+ db_strings::right_parenthesis
		+ db_strings::values + db_strings::left_parenthesis
		+ db_strings::escape_char + smi.server_id + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.company_account + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.subaccount + db_strings::escape_char
		+ db_strings::comma + datatype_convert<std::string>((unsigned int)smi.state)
		+ db_strings::comma + datatype_convert<std::string>(smi.sn_by_client)
		+ db_strings::comma + datatype_convert<std::string>(smi.sn_by_protocol_server)
		+ db_strings::comma + datatype_convert<std::string>(smi.sn_by_gateway)
		+ db_strings::comma + datatype_convert<std::string>(smi.sn_by_provider)
		+ db_strings::comma + db_strings::escape_char + smi.message_id_of_protocol_server + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.message_id_of_provider + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.dst_addr + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.ip + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.session_id + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.gateway_id + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.src_addr + db_strings::escape_char
		+ db_strings::comma + datatype_convert<std::string>((int)smi.ack_status)
		+ db_strings::comma + datatype_convert<std::string>(smi.submit_time_of_client)
		+ db_strings::comma + datatype_convert<std::string>(smi.delivering_time_of_client)
		+ db_strings::comma + datatype_convert<std::string>(smi.delivery_time_of_client)
		+ db_strings::comma + datatype_convert<std::string>(smi.submit_time_of_gateway)
		+ db_strings::comma + datatype_convert<std::string>(smi.delivering_time_of_gateway)
		+ db_strings::comma + datatype_convert<std::string>(smi.delivery_time_of_gateway)
		+ db_strings::comma + db_strings::escape_char + smi.error_status + db_strings::escape_char
		+ db_strings::comma + datatype_convert<std::string>(smi.error_code)
		+ db_strings::comma + datatype_convert<std::string>((int)smi.udhi_reference)
		+ db_strings::comma + datatype_convert<std::string>((int)smi.udhi_total_parts)
		+ db_strings::comma + datatype_convert<std::string>((int)smi.udhi_part_index)
		+ db_strings::comma + datatype_convert<std::string>((int)smi.datacoding_type)
		+ db_strings::comma + db_strings::escape_char + smi.content + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.content_hash + db_strings::escape_char
		+ db_strings::comma + datatype_convert<std::string>((int)smi.charge_mode_of_client)
		+ db_strings::comma + datatype_convert<std::string>(smi.price_of_client)
		+ db_strings::comma + datatype_convert<std::string>((int)smi.charge_mode_of_gateway)
		+ db_strings::comma + datatype_convert<std::string>(smi.price_of_gateway)
		+ db_strings::comma + db_strings::escape_char + smi.mcc + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.mnc + db_strings::escape_char
		+ db_strings::comma + datatype_convert<std::string>((int)smi.classification)
		+ db_strings::comma + datatype_convert<std::string>((int)smi.valid)
		+ db_strings::comma + db_strings::escape_char + smi.protocol_of_client + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.protocol_of_gateway + db_strings::escape_char
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

bool records_db::peek_smis(const std::string& table_name, int to_peek, std::vector<cb_sm_information>& smis)
{
	std::string select_string =  db_strings::select + db_strings::star_char + db_strings::from
	+ table_name
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
		if(mysql_num_fields(res_set) == 40)
		{
			cb_sm_information smi;
			smi.server_id = row[0];
			smi.company_account = row[1];
			smi.subaccount = row[2];
			smi.state = (SMI_STATE)datatype_convert<int>(row[3]);
			smi.sn_by_client = datatype_convert<unsigned int>(row[4]);
			smi.sn_by_protocol_server = datatype_convert<unsigned int>(row[5]);
			smi.sn_by_gateway = datatype_convert<unsigned int>(row[6]);
			smi.sn_by_provider = datatype_convert<unsigned int>(row[7]);
			smi.message_id_of_protocol_server = row[8];
			smi.message_id_of_provider = row[9];
			smi.dst_addr = row[10];
			smi.ip = row[11];
			smi.session_id = row[12];
			smi.gateway_id = row[13];
			smi.src_addr = row[14];
			smi.ack_status = (ACK_STATUS)datatype_convert<int>(row[15]);
			smi.submit_time_of_client = datatype_convert<boost::uint64_t>(row[16]);
			smi.delivering_time_of_client = datatype_convert<boost::uint64_t>(row[17]);
			smi.delivery_time_of_client = datatype_convert<boost::uint64_t>(row[18]);
			smi.submit_time_of_gateway = datatype_convert<boost::uint64_t>(row[19]);
			smi.delivering_time_of_gateway = datatype_convert<boost::uint64_t>(row[20]);
			smi.delivery_time_of_gateway = datatype_convert<boost::uint64_t>(row[21]);
			smi.error_status = row[22];
			smi.error_code = datatype_convert<unsigned int>(row[23]);
			smi.udhi_reference = (unsigned char)datatype_convert<int>(row[24]);
			smi.udhi_total_parts = (unsigned char)datatype_convert<int>(row[25]);
			smi.udhi_part_index = (unsigned char)datatype_convert<int>(row[26]);
			smi.datacoding_type = (unsigned char)datatype_convert<int>(row[27]);
			smi.content = row[28];
			smi.content_hash = row[29];
			smi.charge_mode_of_client = datatype_convert<int>(row[30]);
			smi.price_of_client = datatype_convert<double>(row[31]);
			smi.charge_mode_of_gateway = datatype_convert<int>(row[32]);
			smi.price_of_gateway = datatype_convert<double>(row[33]);
			smi.mcc = row[34];
			smi.mnc = row[35];
			smi.classification = (unsigned char)datatype_convert<int>(row[36]);
			smi.valid = (unsigned char)datatype_convert<int>(row[37]);
			smi.protocol_of_client = row[38];
			smi.protocol_of_gateway = row[39];
			smis.push_back(smi);
		}
	}
	mysql_free_result(res_set);
	return true;
}

bool records_db::delete_smis(const std::string& table_name, const std::vector<cb_sm_information>& smis)
{
	close_auto_commit();
	for(size_t i = 0; i < smis.size(); i++)
	{
		const cb_sm_information& smi = smis.at(i);
		std::string delete_string =  db_strings::delete_from
		+ table_name
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
bool records_db::get_smis(const std::string& table_name, int to_get, std::vector<cb_sm_information>& smis)
{
	boost::mutex::scoped_lock lock(db_mutex_);
	peek_smis(table_name, to_get, smis);
	delete_smis(table_name, smis);
	return true;
}

bool records_db::insert_mos(const std::vector<cb_sm_information> &smis)
{
	boost::mutex::scoped_lock lock(db_mutex_);
	{
		initialize_tables();
	}
	close_auto_commit();
	
	for(size_t i = 0; i < smis.size(); i++)
	{
		const cb_sm_information& smi = smis.at(i);
		std::string insert_string =  db_strings::insert_into
		+ mos_table_name_ + db_strings::left_parenthesis
		+ db_strings::server_id
		+ db_strings::comma + db_strings::company_account
		+ db_strings::comma + db_strings::subaccount
		+ db_strings::comma + db_strings::src_addr
		+ db_strings::comma + db_strings::dst_addr
		+ db_strings::comma + db_strings::sn_by_protocol_server
		+ db_strings::comma + db_strings::sn_by_provider
		+ db_strings::comma + db_strings::delivery_time_of_gateway
		+ db_strings::comma + db_strings::delivery_time_of_client
		+ db_strings::comma + db_strings::udhi_reference
		+ db_strings::comma + db_strings::udhi_total_parts
		+ db_strings::comma + db_strings::udhi_part_index
		+ db_strings::comma + db_strings::datacoding_type
		+ db_strings::comma + db_strings::content
		+ db_strings::comma + db_strings::content_hash
		+ db_strings::comma + db_strings::protocol_of_gateway
		+ db_strings::right_parenthesis
		+ db_strings::values + db_strings::left_parenthesis
		+ db_strings::escape_char + smi.server_id + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.company_account + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.subaccount + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.src_addr + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.dst_addr + db_strings::escape_char
		+ db_strings::comma + datatype_convert<std::string>(smi.sn_by_protocol_server)
		+ db_strings::comma + datatype_convert<std::string>(smi.sn_by_provider)
		+ db_strings::comma + datatype_convert<std::string>(smi.delivery_time_of_gateway)
		+ db_strings::comma + datatype_convert<std::string>(smi.submit_time_of_gateway)
		+ db_strings::comma + datatype_convert<std::string>((int)smi.udhi_reference)
		+ db_strings::comma + datatype_convert<std::string>((int)smi.udhi_total_parts)
		+ db_strings::comma + datatype_convert<std::string>((int)smi.udhi_part_index)
		+ db_strings::comma + datatype_convert<std::string>((int)smi.datacoding_type)
		+ db_strings::comma + db_strings::escape_char + smi.content + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.content_hash + db_strings::escape_char
		+ db_strings::comma + db_strings::escape_char + smi.protocol_of_gateway + db_strings::escape_char
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

#else
records_db::records_db()
{
}
records_db::~records_db()
{
}
std::string records_db::generate_dlrs_table_name()
{
	return "xxxx";
}
std::string records_db::generate_mos_table_name()
{
	return "xxxx";
}
bool records_db::initialize_tables()
{
	return true;
}

bool records_db::do_connect(const std::string& host_name, const std::string& user_name, const std::string& password,const std::string& db_name)
{
	return true;
}
//do not forget to call mysql_free_result to free *res_set
bool records_db::process_select(const std::string& query_string, MYSQL_RES** res_set)
{
	return true;
}
void records_db::close_auto_commit()
{
}
void records_db::auto_commit(bool suc)
{
}
bool records_db::insert_smis(const std::vector<cb_sm_information> &smis)
{
	return true;
}

bool records_db::peek_smis(const std::string& table_name, int to_peek, std::vector<cb_sm_information>& smis)
{
	return true;
}

bool records_db::delete_smis(const std::string& table_name, const std::vector<cb_sm_information>& smis)
{
	return true;
	
}
bool records_db::get_smis(const std::string& table_name, int to_get, std::vector<cb_sm_information>& smis)
{
	return true;
}

bool records_db::insert_mos(const std::vector<cb_sm_information> &smis)
{
	return true;
}
#endif
