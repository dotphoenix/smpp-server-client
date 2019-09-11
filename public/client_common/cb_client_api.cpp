//
//  cb_client_api.cpp
//  client_common
//
//  Created by Alex on 5/20/15.
//  Copyright (c) 2015 BiFang technology. All rights reserved.
//
#include "algorithm.hpp"
#include "cb_client_api.h"
#include "clients_manager.h"
#include "configer.hpp"
#include "json_parser.h"
#include "logger.hpp"
#include "md5.h"
#include "smpp_datacoding_toolkit.h"
#include "configer.hpp"
#include <fstream>
#include <boost/thread/mutex.hpp>
#include "short_message_db.h"
#include "clients_statistic.hpp"
#include "data_encoding_tools.hpp"

static void read_log_config(const std::string& configer_file, bool& to_console, LEVEL& level)
{
	if(boost::filesystem::exists(configer_file))
	{
		configer conf(configer_file);
		std::string sec_name = "log";
		to_console = (conf.get_int(sec_name, "console", 1) == 1);
		level = (LEVEL)(conf.get_int(sec_name, "level", 3));//3 is debug, 2 is warning
	}
	else
	{
		to_console = true;
		level = LEVEL_DEBUG;
	}
}
static void read_disorder_content_mode(const std::string& configer_file, int& mode)
{
	if(boost::filesystem::exists(configer_file))
	{
		configer conf(configer_file);
		std::string sec_name = "disorder_content";
		mode = conf.get_int(sec_name, "mode", 0);
	}
	else
	{
		mode = 0;
	}
}
static bool to_console = false;
static std::string current_log_file_path;
static std::ofstream logger_ofs;
static bool record_all_to_db = true;
static int disorder_content_mode; //修改短信内容，0表示不改，1表示一种改法，2表示一种改法

static std::string get_log_file_path()
{
	std::string logfile = logger_get_path() + "cb_client_";
	time_t tt;
	time(&tt);
	tm* t = localtime(&tt);
	logfile += format_uint32_to_lenght((unsigned int)t->tm_year + 1900, 4);
	logfile += format_uint32_to_lenght((unsigned int)t->tm_mon + 1, 2);
	logfile += format_uint32_to_lenght((unsigned int)t->tm_mday, 2);
	//logfile += format_uint32_to_lenght((unsigned int)t->tm_hour, 2);
	logfile += ".log";
	return logfile;
}
static void smi_to_sm_status( const cb_sm_information& smi, sm_status& ss)
{
	ss.session_id_ = smi.session_id;
	ss.deliver_time_ = smi.delivery_time_of_client;
	ss.dst_ = smi.dst_addr;
	ss.error_code_ = smi.error_code;
	ss.status_ = smi.error_status;
	ss.username_ = smi.company_account;
	ss.message_id_ = smi.message_id_of_protocol_server;
	ss.udhi_reference_ = smi.udhi_reference;
	ss.udhi_total_parts_ = smi.udhi_total_parts;
	ss.udhi_part_index_ = smi.udhi_part_index;
}
void smi_to_sm_mo(const cb_sm_information& smi, sm_mo& mo)
{
	mo.src_addr_ = smi.src_addr;
	mo.dst_addr_ = smi.dst_addr;
	mo.dc_ = smi.datacoding_type;
	mo.content_ = smi.content;
	mo.deliver_time_ = smi.delivery_time_of_client;
}
static boost::mutex dlr_db_mutex;
static boost::mutex mo_db_mutex;
static std::string get_dlrs_db_path()
{
	std::string path = "dlrs.db";
	return path;
}
static std::string get_mos_db_path()
{
	std::string path = "mos.db";
	return path;
}

void record_dlr_to_db(const cb_sm_information& smi)
{
	if(record_all_to_db)
	{
		std::vector<cb_sm_information> smis;
		smis.push_back(smi);
		boost::mutex::scoped_lock lock(dlr_db_mutex);
		short_message_db db(get_dlrs_db_path());
		db.insert(smis);
	}
}
void record_mos_to_db(const cb_sm_information & smi)
{
	if(record_all_to_db)
	{
		std::vector<cb_sm_information> smis;
		smis.push_back(smi);
		boost::mutex::scoped_lock lock(mo_db_mutex);
		short_message_db db(get_mos_db_path());
		db.insert(smis);
	}
}
void cb_client_query_mos(boost::uint64_t begin_time, boost::uint64_t end_time, std::vector<sm_mo>& mos)
{
	boost::mutex::scoped_lock lock(mo_db_mutex);
	std::vector<cb_sm_information> smis;
	short_message_db db(get_mos_db_path());
	std::vector<sm_db_pair> pairs;
	db.query_submit_time_between(begin_time, end_time, smis, pairs);
	for(size_t i = 0; i < smis.size(); i++)
	{
		sm_mo mo;
		smi_to_sm_mo(smis.at(i), mo);
		mos.push_back(mo);
	}
}

void cb_client_query_dlrs(boost::uint64_t begin_time, boost::uint64_t end_time, std::vector<sm_status>& dlrs)
{
	boost::mutex::scoped_lock lock(dlr_db_mutex);
	std::vector<cb_sm_information> smis;
	short_message_db db(get_dlrs_db_path());
	std::vector<sm_db_pair> pairs;
	db.query_submit_time_between(begin_time, end_time, smis, pairs);
	for(size_t i = 0; i < smis.size(); i++)
	{
		sm_status ss;
		smi_to_sm_status(smis.at(i), ss);
		dlrs.push_back(ss);
	}
}
void client_log_function(const char* message)
{
	if(to_console)
	{
#if defined _DEBUG && defined WIN32
			OutputDebugStringA(message);
#endif
		 	std::cout<<std::string(message)<<std::endl;
	}
	else
	{
		std::string logfile = get_log_file_path();
		if(logfile.compare(current_log_file_path) != 0)
		{
			if(logger_ofs.is_open())
			{
				logger_ofs.close();
			}
			current_log_file_path = logfile;
			logger_ofs.open(logfile.c_str(), std::ios_base::out | std::ios_base::app);
		}
		logger_ofs.write(message, strlen(message));
		logger_ofs.flush();
	}
}
void cb_client_start(const std::string& configer_file, bool record_to_db)
{
	LEVEL level;
	read_log_config(configer_file, to_console, level);
	read_disorder_content_mode(configer_file, disorder_content_mode);
	logger_init(level, client_log_function);
	record_all_to_db = record_to_db;
	clients_manager::get_instance()->add_clients_from_file(configer_file);
}
bool cb_client_is_bound()
{
	return clients_manager::get_instance()->is_protocol_connected("");
}
bool cb_client_status(const std::string& sysid)
{
	return true;
}
void cb_client_stop()
{
	clients_manager::get_instance()->stop_all();
	clients_manager::free_instance();
	logger_uninit();

	if(logger_ofs.is_open())
	{
		logger_ofs.close();
	}
}

void cb_client_get_dlrs(int to_get, std::vector<sm_status>& dlrs)
{
	std::vector<cb_sm_information> smis;
	clients_manager::get_instance()->get_global_status(to_get, smis);
	dlrs.clear();
	size_t count = smis.size();
	if(count > 0)
	{
		logger_log(__FUNCTION__, LEVEL_INFO, "total get %d dlrs.", count);
		for(size_t i = 0; i < count; i++)
		{
			sm_status ss;
			smi_to_sm_status(smis.at(i), ss);
			logger_log(__FUNCTION__, LEVEL_INFO, "dlr %d, message id:%s, session id: %s, dst: %s, udhi ref: %d, udhi index: %d", i, ss.message_id_.c_str(), ss.session_id_.c_str(), ss.dst_.c_str(), (int)ss.udhi_reference_, (int)ss.udhi_part_index_);
			dlrs.push_back(ss);
		}
	}
}
void cb_client_get_dlrs(int to_get, std::string& json)
{
	std::vector<sm_status> dlrs;
	json = "";
	cb_client_get_dlrs(to_get, dlrs);
	if(dlrs.size() > 0)
	{
		json = json_parser::generate_global_status(dlrs);
	}
}

void cb_client_get_mos(std::vector<sm_mo>& mos)
{
	std::vector<cb_sm_information> smis;
	mos.clear();
	clients_manager::get_instance()->get_mos(smis);
	size_t count = smis.size();
	if(count > 0)
	{
		logger_log(__FUNCTION__, LEVEL_INFO, "total get %d mos.", count);
		for(size_t i = 0; i < count; i++)
		{
			sm_mo mo;
			smi_to_sm_mo(smis.at(i), mo);
			logger_log(__FUNCTION__, LEVEL_INFO, "mo %d, src address:%s, dst address: %s, delivery time: %d, content: %s", i, mo.src_addr_.c_str(), mo.dst_addr_.c_str(), mo.deliver_time_, mo.content_.c_str());
			mos.push_back(mo);
		}
	}
}

void cb_client_get_mos(std::string& json)
{
	std::vector<sm_mo> mos;
	json = "";
	cb_client_get_mos(mos);
	if(mos.size() > 0)
	{
		json = json = json_parser::generate_mos(mos);
	}
}

void cb_client_get_statistic_string(std::string& s)
{
	s = clients_statistic::get_instance()->statistic();
}

std::string cb_client_private_transform_json_to_smis(const std::string& json, std::vector<cb_sm_information>& smis)
{
	std::string session_id = generate_message_id();
	std::vector<sm_info> sis;
	smis.clear();
	json_parser::parse_smss(std::string(json), sis);
	for(size_t i = 0; i < sis.size(); i++)
	{
		sm_info& si = sis.at(i);
		std::string dst = si.dst_;
		std::string sep = ";";
		std::vector<std::string> dsts;
		split_by_separator(si.dst_, sep, dsts);
		for(size_t j = 0; j < dsts.size(); j++)
		{
			cb_sm_information smi;
			if(dsts.at(j).length() == 0)
			{
				continue;
			}
			std::string dst_addr = dsts.at(j);
			if(dst_addr.find_first_not_of("+0123456789") != std::string::npos)
			{
				logger_log(__FUNCTION__, LEVEL_WARNING, "%s is not a valid phone number", dst_addr.c_str());
				continue;
			}
			smi.dst_addr = dst_addr;
			smi.content = si.msg_;
			smi.src_addr = si.src_;
			smi.submit_time_of_client = (boost::uint64_t)time(NULL);
			smi.datacoding_type = si.datacoding_;
			smi.udhi_reference = si.udhi_reference_;
			smi.udhi_total_parts = si.udhi_total_parts_;
			smi.udhi_part_index = si.udhi_part_index_;
			smi.session_id = session_id;
			smi.sn_by_client = generate_sn();
			smi.price_of_client = 0;
			smi.classification = smi.valid = 1;
			smi.delivery_time_of_client = smi.delivering_time_of_client = 0;
			smi.error_code = 0;
			generate_hash_md5((const unsigned char *)smi.content.c_str(), (unsigned int)smi.content.length(), smi.content_hash);
			smis.push_back(smi);
			//logger_log(__FUNCTION__, LEVEL_DEBUG, "parsed a smi, dst: %s, src: %s, content: %s", smi.dst_addr.c_str(), smi.src_addr.c_str(), smi.content.c_str());
		}
	}
	return session_id;
}
bool cb_client_private_submit_smis(std::vector<cb_sm_information>& smis, const std::string& failed_reason)
{
	return clients_manager::get_instance()->submit_smis(smis, failed_reason);
}
std::string cb_client_submit_sms(const std::string& json)
{
	std::vector<cb_sm_information> smis;
	std::string session_id = cb_client_private_transform_json_to_smis(json, smis);
	if(smis.size() > 0)
	{
		if(cb_client_private_submit_smis(smis, std::string("")))
		{
			return session_id;
		}
		else
		{
			return "";
		}
	}
	else
	{
		return "";
	}
}

int cb_client_format_sms(const std::string& numbers, const std::string& content, const std::string& src_addr, int count_of_batch, std::vector<std::string>& jsons)
{
	if((numbers.find_first_not_of("+0123456789;") != std::string::npos)
	   || (numbers.length() == 0)
	   || (numbers.length() == 0))
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "destination addresses are not valid:%s, content is: %s", numbers.c_str(), content.c_str());
		return 0;
	}
	std::string sep = ";";
	std::vector<std::string> dsts;
	split_by_separator(numbers, sep, dsts);
	std::vector<sm_info> sis;
	int total = 0;
	std::vector<splitted_sm> ssms;
	if(disorder_content_mode == 0)
	{
		smpp_datacoding_split_long_sm(content, ssms); //split long short message
	}
	for(size_t i = 0; i < dsts.size(); i++)
	{
		std::string dst = dsts.at(i);
		if(dst.length() == 0)
		{
			continue;
		}
		if(disorder_content_mode == 1)
		{
			std::string new_content = append_header_to_string(content, dst);
			ssms.clear();
			smpp_datacoding_split_long_sm(new_content, ssms); //split long short message
		}
		else if(disorder_content_mode == 2)
		{
			std::string new_content = insert_char_to_string(content, i);
			ssms.clear();
			smpp_datacoding_split_long_sm(new_content, ssms); //split long short message
		}
		for(size_t j = 0; j < ssms.size(); j++)
		{
			sis.push_back(sm_info(dst, ssms.at(j).content_, src_addr, ssms.at(j).dc_, ssms.at(j).udhi_ref_,
								  ssms.at(j).udhi_total_, ssms.at(j).udhi_index_));
		}
		if((int)sis.size() >= count_of_batch)
		{
			std::string json = json_parser::generate_smss(sis);
			total += (int)sis.size();
			jsons.push_back(json);
			sis.clear();
		}
	}
	if(sis.size() > 0)
	{
		std::string json = json_parser::generate_smss(sis);
		total += (int)sis.size();
		jsons.push_back(json);
	}
	return total;
}
