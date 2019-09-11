//
//  short_message_db.cpp
//  ICBCPlatform
//
//  Created by Alex on 11/27/14.
//  Copyright (c) 2014 BiFang technology. All rights reserved.
//
#include "algorithm.hpp"
#include "short_message_db.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include "logger.hpp"

short_message_db::short_message_db(const std::string& filename)
{
	boost::filesystem::path rootpath = boost::filesystem::initial_path<boost::filesystem::path>();
	boost::filesystem::path databasepath = rootpath / "database";
	databasepath += boost::filesystem::path::preferred_separator;
	if(!boost::filesystem::exists(databasepath))
	{
		try
		{
			boost::filesystem::create_directory(databasepath);
		}
		catch (std::exception& ex)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR,"exception: %s.", ex.what());
		}
	}
	filename_ = databasepath.string()  + filename;
	
	table_ = "sm_table";
	
	item_company_account_ = "company_account";
	item_subaccount_ = "subaccount";
	item_session_id_ = "session_id";
	item_sn_ = "sn";
	item_message_id_ = "msg_id";
	item_src_addr_ = "src_addr";
	item_dst_addr_ = "dst_addr";
	item_submit_time_ = "submit_time";
	item_delivering_time_ = "delivering_time";
	item_delivery_time_ = "delivery_time";
	item_datacoding_ = "datacoding";
	item_hash_ = "hash";
	item_msg_ = "msg";
	item_error_status_ = "error_status";
	item_error_code_ = "error_code";
	item_udhi_reference_ = "udhi_reference";
	item_udhi_total_parts_ = "udhi_total_parts";
	item_udhi_part_index_ = "udhi_part_index";
	
	std::string create_table_sql = "create table if not exists ";
	std::string v1 = " VARCHAR(32) ";
	std::string v2 = " VARCHAR(1024) ";
	std::string v3 = " VARCHAR(8) ";
	int status;
	
	if(!boost::filesystem::exists(filename_))
	{
		sqlite3pp::database db(filename_.c_str());
		try
		{
			create_table_sql = create_table_sql + table_ + "(" + item_company_account_ + v1 + " , ";
			create_table_sql = create_table_sql + item_subaccount_   + v1 + ", ";
			create_table_sql = create_table_sql + item_session_id_   + v1 + ", ";
			create_table_sql = create_table_sql + item_sn_   + v1 + ", ";
			create_table_sql = create_table_sql + item_message_id_   + v1 + ", ";
			create_table_sql = create_table_sql + item_src_addr_   + v1 + ", ";
			create_table_sql = create_table_sql + item_dst_addr_   + v1 + ", ";
			create_table_sql = create_table_sql + item_submit_time_   + v1 + ", ";
			create_table_sql = create_table_sql + item_delivering_time_   + v1 + ", ";
			create_table_sql = create_table_sql + item_delivery_time_   + v1 + ", ";
			create_table_sql = create_table_sql + item_datacoding_   + v3 + ", ";
			create_table_sql = create_table_sql + item_hash_   + v1 + ", ";
			create_table_sql = create_table_sql + item_msg_   + v2 + ", ";
			create_table_sql = create_table_sql + item_error_code_   + v1 + ", ";
			create_table_sql = create_table_sql + item_error_status_   + v1 + ", ";
			create_table_sql = create_table_sql + item_udhi_reference_   + v3 + ", ";
			create_table_sql = create_table_sql + item_udhi_total_parts_   + v3 + ", ";
			create_table_sql = create_table_sql + item_udhi_part_index_   + v3 + ") ";
			status = db.execute(create_table_sql.c_str());
			if(status != 0)
			{
				logger_log(__FUNCTION__, LEVEL_ERROR, "create table %s failed, db is %s , status is: %d " ,
						   table_.c_str() , filename_.c_str(), status);
			}
			/*
			status = db.execute("PRAGMA synchronous = OFF");
			status = db.execute("PRAGMA journal_mode = MEMORY");
			if(status != 0)
			{
				logger_log(__FUNCTION__, LEVEL_ERROR, "set \"synchronous = OFF\" of db %s failed, status is: %d " , filename_.c_str(), status);
			}
			*/
		}
		catch (std::exception& ex)
		{
			std::string what = ex.what();
			logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error: %s ." , what.c_str());
		}
	}
}

bool short_message_db::insert(const std::vector<cb_sm_information>& smis)
{
	try
	{
		WORK_START
		sqlite3pp::database db(filename_.c_str());
		sqlite3pp::transaction xct(db);
		
		std::string insert_default_sql = "INSERT INTO " + table_ + " ( "
		+ item_company_account_ + "," + item_subaccount_ + ","+ item_session_id_  + "," + item_sn_  + "," + item_message_id_ + "," + item_src_addr_ + ","+ item_dst_addr_ + ","+ item_submit_time_ + "," + item_delivering_time_ + ","+ item_delivery_time_ +   ","+ item_datacoding_  + ","+ item_hash_ + ","+ item_msg_  + ","+ item_error_code_  + "," + item_error_status_ + ","+ item_udhi_reference_  + "," + item_udhi_total_parts_ + "," + item_udhi_part_index_ + ")  VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
		int status;
		for(size_t i = 0; i < smis.size(); i++)
		{
			sqlite3pp::command cmd(db,  insert_default_sql.c_str());
			cmd.bind(1, smis.at(i).company_account.c_str());
			cmd.bind(2, smis.at(i).systemid.c_str());
			cmd.bind(3, smis.at(i).session_id.c_str());
			cmd.bind(4, (int)smis.at(i).sn_by_client);
			cmd.bind(5, smis.at(i).message_id_of_protocol_server.c_str());
			cmd.bind(6, smis.at(i).src_addr.c_str());
			cmd.bind(7, smis.at(i).dst_addr.c_str());
			cmd.bind(8, (long long int)smis.at(i).submit_time_of_client);
			cmd.bind(9, (long long int)smis.at(i).delivering_time_of_client);
			cmd.bind(10, (long long int)smis.at(i).delivery_time_of_client);
			cmd.bind(11, (int)smis.at(i).datacoding_type);
			cmd.bind(12, smis.at(i).content_hash.c_str());
			cmd.bind(13, smis.at(i).content.c_str());
			cmd.bind(14, (int)smis.at(i).error_code);
			cmd.bind(15, smis.at(i).error_status.c_str());
			cmd.bind(16, (int)smis.at(i).udhi_reference);
			cmd.bind(17, (int)smis.at(i).udhi_total_parts);
			cmd.bind(18, (int)smis.at(i).udhi_part_index);
			
			status = cmd.execute();
			if(status != 0)
			{
				logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error, execute returns %d ." , status);
			}
		}
		
		status = xct.commit();
		if(status != 0)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error, commit returns %d ." , status);
			return false;
		}
		char msg[1024];
		sprintf(msg, "insert %d smis to %s", (int)smis.size(), filename_.c_str());
		WORK_FINISH(msg)
		return true;
	}
	catch (std::exception& ex)
	{
		std::string what = ex.what();
		logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error: %s ." , what.c_str());
		return false;
	}
}
bool short_message_db::read_smi(sqlite3pp::query::iterator& i, cb_sm_information& smi)
{
	smi.company_account = (*i).get< char const*>(0);
	smi.systemid = (*i).get< char const*>(1);
	smi.session_id = (*i).get< char const*>(2);
	smi.sn_by_client = (unsigned int)(*i).get< int>(3);
	smi.message_id_of_protocol_server = (*i).get< char const*>(4);
	smi.src_addr = (*i).get< char const*>(5);
	smi.dst_addr = (*i).get< char const*>(6);
	smi.submit_time_of_client = (boost::uint64_t)(*i).get< long long int>(7);
	smi.delivering_time_of_client = (boost::uint64_t)(*i).get< long long int>(8);
	smi.delivery_time_of_client = (boost::uint64_t)(*i).get< long long int>(9);
	smi.datacoding_type = (unsigned char)(*i).get< int>(10);
	smi.content_hash = (*i).get< char const*>(11);
	smi.content = (*i).get< char const*>(12);
	smi.error_code = (*i).get< int>(13);
	smi.error_status = (*i).get< char const*>(14);
	smi.udhi_reference = (unsigned char)(*i).get< int>(15);
	smi.udhi_total_parts = (unsigned char)(*i).get< int>(16);
	smi.udhi_part_index = (unsigned char)(*i).get< int>(17);
	return true;
}

bool short_message_db::get(std::vector<cb_sm_information>& smis, const std::vector<sm_db_pair> &get_decide_pairs, const std::vector<std::string>& delete_decide_items)
{
	try
	{
		WORK_START
		{
			sqlite3pp::database db(filename_.c_str());
			std::string qry_sql = "SELECT * FROM "+ table_;
			size_t index = 0;
			for(index = 0; index < get_decide_pairs.size(); index++)
			{
				if(index == 0)
				{
					qry_sql += " WHERE ";
				}
				if(index > 0)
				{
					qry_sql += " and ";
				}
				qry_sql = qry_sql + get_decide_pairs.at(index).key + "='" + get_decide_pairs.at(index).value + "' ";
			}
			
			sqlite3pp::query qry(db,  qry_sql.c_str());
			sqlite3pp::query::iterator i = qry.begin();
			if(i == qry.end())
			{
				return false;
			}
			for (; i != qry.end(); ++i)
			{
				cb_sm_information smi;
				read_smi(i, smi);
				smis.push_back(smi);
			}
			if(smis.size() == 0)
			{
				return true;
			}
		}
		bool deleted = false;
		{
			size_t index = 0;
			std::vector< std::vector<sm_db_pair> > v_pairs;
			for(index = 0; index < smis.size(); index++)
			{
				std::vector<sm_db_pair> pairs;
				for(size_t j = 0; j < delete_decide_items.size(); j++)
				{
					sm_db_pair pair;
					pair.key = delete_decide_items.at(j);
					pair.value = get_value(smis.at(index), pair.key);
					pairs.push_back(pair);
				}
				v_pairs.push_back(pairs);
			}
			deleted = delete_internal(v_pairs);
		}
		char msg[1024];
		sprintf(msg, "get %d smis and delete thme of %s", (int)smis.size(), filename_.c_str());
		WORK_FINISH(msg)
		return deleted;
	}
	catch (std::exception& ex)
	{
		std::string what = ex.what();
		logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error: %s ." , what.c_str());
		return false;
	}
}
bool short_message_db::get(int max_get, std::vector<cb_sm_information>& smis, const std::vector<sm_db_pair> &get_decide_pairs, const std::vector<std::string>& delete_decide_items)
{
	try
	{
		WORK_START
		{
			sqlite3pp::database db(filename_.c_str());
			std::string qry_sql = "SELECT * FROM "+ table_;
			size_t index = 0;
			for(index = 0; index < get_decide_pairs.size(); index++)
			{
				if(index == 0)
				{
					qry_sql += " WHERE ";
				}
				if(index > 0)
				{
					qry_sql += " and ";
				}
				qry_sql = qry_sql + get_decide_pairs.at(index).key + "='" + get_decide_pairs.at(index).value + "' ";
			}
			qry_sql = qry_sql + " LIMIT " + format_decimal_to_string((unsigned int)max_get);
			sqlite3pp::query qry(db,  qry_sql.c_str());
			sqlite3pp::query::iterator i = qry.begin();
			if(i == qry.end())
			{
				return false;
			}
			for (; i != qry.end(); ++i)
			{
				cb_sm_information smi;
				read_smi(i, smi);
				smis.push_back(smi);
			}
			if(smis.size() == 0)
			{
				return true;
			}
		}
		bool deleted = false;
		{
			size_t index = 0;
			std::vector< std::vector<sm_db_pair> > v_pairs;
			for(index = 0; index < smis.size(); index++)
			{
				std::vector<sm_db_pair> pairs;
				for(size_t j = 0; j < delete_decide_items.size(); j++)
				{
					sm_db_pair pair;
					pair.key = delete_decide_items.at(j);
					pair.value = get_value(smis.at(index), pair.key);
					pairs.push_back(pair);
				}
				v_pairs.push_back(pairs);
			}
			deleted = delete_internal(v_pairs);
		}
		char msg[1024];
		sprintf(msg, "get %d smis and delete them of %s ", (int)smis.size(), filename_.c_str());
		WORK_FINISH(msg)
		return deleted;
	}
	catch (std::exception& ex)
	{
		std::string what = ex.what();
		logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error: %s ." , what.c_str());
		return false;
	}
}
bool short_message_db::get_submit_time_before(boost::uint64_t submit_time, std::vector<cb_sm_information>& smis, const std::vector<std::string>& delete_decide_items)
{
	try
	{
		WORK_START
		{
			sqlite3pp::database db(filename_.c_str());
			std::string qry_sql = "SELECT * FROM "+ table_;
			qry_sql += " WHERE ";
			qry_sql = qry_sql + item_submit_time_ + " < " + datatype_convert<std::string>(submit_time) + " ";
			
			sqlite3pp::query qry(db,  qry_sql.c_str());
			sqlite3pp::query::iterator i = qry.begin();
			if(i == qry.end())
			{
				return false;
			}
			for (; i != qry.end(); ++i)
			{
				cb_sm_information smi;
				read_smi(i, smi);
				smis.push_back(smi);
			}
		}
		bool deleted = false;
		{
			size_t index = 0;
			std::vector< std::vector<sm_db_pair> > v_pairs;
			for(index = 0; index < smis.size(); index++)
			{
				std::vector<sm_db_pair> pairs;
				for(size_t j = 0; j < delete_decide_items.size(); j++)
				{
					sm_db_pair pair;
					pair.key = delete_decide_items.at(j);
					pair.value = get_value(smis.at(index), pair.key);
					pairs.push_back(pair);
				}
				v_pairs.push_back(pairs);
			}
			deleted = delete_internal(v_pairs);
		}
		char msg[1024];
		sprintf(msg, "get %d smis and delete them of %s ", (int)smis.size(), filename_.c_str());
		WORK_FINISH(msg)
		return deleted;
	}
	catch (std::exception& ex)
	{
		std::string what = ex.what();
		logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error: %s ." , what.c_str());
		return false;
	}
}
bool short_message_db::query_submit_time_between(boost::uint64_t begin_time, boost::uint64_t end_time, std::vector<cb_sm_information>& smis, const std::vector<sm_db_pair> &query_decide_pairs)
{
	try
	{
		WORK_START
		{
			sqlite3pp::database db(filename_.c_str());
			std::string qry_sql = "SELECT * FROM "+ table_;
			qry_sql += " WHERE ";
			for(size_t index = 0; index < query_decide_pairs.size(); index++)
			{
				if(index > 0)
				{
					qry_sql += " AND ";
				}
				qry_sql = qry_sql + query_decide_pairs.at(index).key + "='" + query_decide_pairs.at(index).value + "' ";
			}
			if(query_decide_pairs.size() > 0)
			{
				qry_sql += " AND ";
			}
			qry_sql = qry_sql + item_delivery_time_ + " > " + datatype_convert<std::string>(begin_time) + " ";
			qry_sql += " AND ";
			qry_sql = qry_sql + item_delivery_time_ + " < " + datatype_convert<std::string>(end_time) + " ";

			sqlite3pp::query qry(db,  qry_sql.c_str());
			sqlite3pp::query::iterator i = qry.begin();
			if(i == qry.end())
			{
				return false;
			}
			for (; i != qry.end(); ++i)
			{
				cb_sm_information smi;
				read_smi(i, smi);
				smis.push_back(smi);
			}
		}

		char msg[1024];
		sprintf(msg, "query %d smis of %s ", (int)smis.size(), filename_.c_str());
		WORK_FINISH(msg)
		return true;
	}
	catch (std::exception& ex)
	{
		std::string what = ex.what();
		logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error: %s ." , what.c_str());
		return false;
	}
}
std::string short_message_db::get_value(const cb_sm_information& smi, const std::string& item_name)
{
	if(item_name.compare(item_company_account_) == 0)
	{
		return smi.company_account;
	}
	else if(item_name.compare(item_subaccount_) == 0)
	{
		return smi.systemid;
	}
	else if(item_name.compare(item_sn_) == 0)
	{
		return datatype_convert<std::string>(smi.sn_by_client);
	}
	else if(item_name.compare(item_message_id_) == 0)
	{
		return smi.message_id_of_protocol_server;
	}
	else if(item_name.compare(item_src_addr_) == 0)
	{
		return smi.src_addr;
	}
	else if(item_name.compare(item_dst_addr_) == 0)
	{
		return smi.dst_addr;
	}
	else if(item_name.compare(item_submit_time_) == 0)
	{
		return datatype_convert<std::string>(smi.submit_time_of_client);
	}
	else if(item_name.compare(item_delivering_time_) == 0)
	{
		return datatype_convert<std::string>(smi.delivering_time_of_client);
	}
	else if(item_name.compare(item_delivery_time_) == 0)
	{
		return datatype_convert<std::string>(smi.delivery_time_of_client);
	}
	else if(item_name.compare(item_datacoding_) == 0)
	{
		return datatype_convert<std::string>((int)smi.datacoding_type);
	}
	else if(item_name.compare(item_hash_) == 0)
	{
		return smi.content_hash;
	}
	else if(item_name.compare(item_msg_) == 0)
	{
		return smi.content;
	}
	else if(item_name.compare(item_error_code_) == 0)
	{
		return datatype_convert<std::string>(smi.error_code);
	}
	else if(item_name.compare(item_error_status_) == 0)
	{
		return smi.error_status;
	}
	else if(item_name.compare(item_session_id_) == 0)
	{
		return smi.session_id;
	}
	return "";
}
bool short_message_db::delete_internal(const std::vector<sm_db_pair>& delete_decide_pairs)
{
	try
	{
		sqlite3pp::database db(filename_.c_str());
		std::string delete_sql = "DELETE FROM "+ table_ + " WHERE ";
		for(size_t i = 0; i < delete_decide_pairs.size(); i++)
		{
			if(i > 0)
			{
				delete_sql += " AND ";
			}
			delete_sql = delete_sql + delete_decide_pairs.at(i).key + "='" + delete_decide_pairs.at(i).value + "' ";
		}
		sqlite3pp::command cmd(db,  delete_sql.c_str());
		int status = db.execute(delete_sql.c_str() );
		
		if(status != 0)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error, execute returns %d ." , status);
			return  false;
		}
		return true;
	}
	catch (std::exception& ex)
	{
		std::string what = ex.what();
		logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error: %s ." , what.c_str());
		return false;
	}
}

bool short_message_db::delete_internal(const std::vector< std::vector<sm_db_pair> >& v_pairs)
{
	try
	{
		WORK_START
		sqlite3pp::database db(filename_.c_str());
		sqlite3pp::transaction xct(db);
		int status;
		for(size_t i = 0; i < v_pairs.size(); i++)
		{
			std::string delete_sql = "DELETE FROM "+ table_ + " WHERE ";
			std::vector<sm_db_pair> delete_decide_pairs = v_pairs.at(i);
			for(size_t i = 0; i < delete_decide_pairs.size(); i++)
			{
				if(i > 0)
				{
					delete_sql += " AND ";
				}
				delete_sql = delete_sql + delete_decide_pairs.at(i).key + "='" + delete_decide_pairs.at(i).value + "' ";
			}
			sqlite3pp::command cmd(db,  delete_sql.c_str());
			status = db.execute(delete_sql.c_str() );
			
			if(status != 0)
			{
				logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error, execute returns %d ." , status);
			}
		}
		status = xct.commit();
		if(status != 0)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error, commit returns %d ." , status);
			return false;
		}
		char msg[1024];
		sprintf(msg, "delete %d smis of %s ", (int)v_pairs.size(), filename_.c_str());
		WORK_FINISH(msg)
		return true;
	}
	catch (std::exception& ex)
	{
		std::string what = ex.what();
		logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error: %s ." , what.c_str());
		return false;
	}
}
bool short_message_db::update_internal(const std::vector<sm_db_pair>& update_decide_pairs, const std::vector<sm_db_pair>& update_pairs)
{
	try
	{
		sqlite3pp::database db(filename_.c_str());
		std::string update_sql = "UPDATE " + table_ + " SET ";
		
		//std::string delete_sql = "DELETE FROM "+ table_ + " WHERE ";
		for(size_t i = 0; i < update_pairs.size(); i++)
		{
			if(i > 0)
			{
				update_sql += " , ";
			}
			update_sql = update_sql + update_pairs.at(i).key + "='" + update_pairs.at(i).value + "' ";
		}
		update_sql += " WHERE ";
		for(size_t j = 0; j < update_decide_pairs.size(); j++)
		{
			if(j > 0)
			{
				update_sql += " AND ";
			}
			update_sql = update_sql + update_decide_pairs.at(j).key + "='" + update_decide_pairs.at(j).value + "' ";
		}
		sqlite3pp::command cmd(db,  update_sql.c_str());
		int status = db.execute(update_sql.c_str() );
		
		if(status != 0)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error, execute returns %d ." , status);
			return  false;
		}
		return true;
	}
	catch (std::exception& ex)
	{
		std::string what = ex.what();
		logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error: %s ." , what.c_str());
		return false;
	}
}

bool short_message_db::fetch_first_and_update(cb_sm_information& smi, const std::vector<sm_db_pair> &fetch_decide_pairs, const std::vector<std::string> &update_decide_items, const std::vector<sm_db_pair> &update_pairs)
{
	try
	{
		WORK_START
		{
			sqlite3pp::database db(filename_.c_str());
			std::string qry_sql = "SELECT * FROM "+ table_ ;
			for(size_t i = 0; i < fetch_decide_pairs.size(); i++)
			{
				if(i == 0)
				{
					qry_sql += " WHERE ";
				}
				if(i > 0)
				{
					qry_sql += " AND ";
				}
				qry_sql = qry_sql + fetch_decide_pairs.at(i).key + "='" + fetch_decide_pairs.at(i).value + "' ";
			}
			qry_sql = qry_sql + " LIMIT 1 ";
			sqlite3pp::query qry1(db,  qry_sql.c_str());
			sqlite3pp::query::iterator i = qry1.begin();
			if(i == qry1.end())
			{
				return false;
			}
			for (; i != qry1.end(); ++i)
			{
				read_smi(i, smi);
				break;
			}
		}
		bool updated = false;
		{
			std::vector<sm_db_pair> update_decide_pairs;
			for(size_t i = 0; i < update_decide_items.size(); i++)
			{
				sm_db_pair pair;
				pair.key = update_decide_items.at(i);
				pair.value = get_value(smi, pair.key);
				update_decide_pairs.push_back(pair);
			}
			updated = update_internal(update_decide_pairs, update_pairs);
		}
		char msg[1024];
		sprintf(msg, "fetch first smi and update it of %s", filename_.c_str());
		WORK_FINISH(msg)
		return updated;
	}
	catch (std::exception& ex)
	{
		std::string what = ex.what();
		logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error: %s ." , what.c_str());
		return false;
	}
}

bool short_message_db::delete_items(const std::vector<sm_db_pair> &delete_decide_pairs)
{
	return delete_internal(delete_decide_pairs);
}
bool short_message_db::read_all(std::vector<cb_sm_information> &smis)
{
	try
	{
		sqlite3pp::database db(filename_.c_str());
		std::string qry_sql = "SELECT * FROM "+ table_;
		
		sqlite3pp::query qry(db,  qry_sql.c_str());
		sqlite3pp::query::iterator i = qry.begin();
		if(i == qry.end())
		{
			return false;
		}
		for (; i != qry.end(); ++i)
		{
			cb_sm_information smi;
			read_smi(i, smi);
			smis.push_back(smi);
		}
		return true;
	}
	catch (std::exception& ex)
	{
		std::string what = ex.what();
		logger_log(__FUNCTION__, LEVEL_ERROR, "sqlite3 error: %s ." , what.c_str());
		return false;
	}
}

bool short_message_db::update(const std::vector<sm_db_pair> &update_decide_pairs, const std::vector<sm_db_pair> &update_pairs)
{
	return update_internal(update_decide_pairs, update_pairs);
}
