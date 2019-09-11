#include "algorithm.hpp"
#include "client_data_source_manager.hpp"
#include <limits>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "cb_protocol_public.hpp"
#include "clients_statistic.hpp"

std::list<client_data_source_ptr> client_data_source_manager::cdsps_;
boost::mutex client_data_source_manager::cdsps_mutex_;
clients_statistic* clients_statistic::instance_ = NULL;
boost::mutex clients_statistic::mutex_;

extern void record_dlr_to_db(const cb_sm_information& smi);
extern void record_mos_to_db(const cb_sm_information & smi);

client_data_source::~client_data_source()
{
	{
		boost::mutex::scoped_lock lock(to_submit_list_mutex_);
		to_submit_list_.clear();
	}
	{
		boost::mutex::scoped_lock lock(delivering_list_mutex_);
		delivering_list_.clear();
	}
	{
		boost::mutex::scoped_lock lock(delivered_list_mutex_);
		delivered_list_.clear();
	}
	{
		boost::mutex::scoped_lock lock(mo_list_mutex_);
		mo_list_.clear();
	}
	{
		boost::mutex::scoped_lock lock(waiting_dlr_timeout_list_mutex_);
		waiting_dlr_timeout_list_.clear();
	}
}
bool client_data_source::is_free()
{
	{
		boost::mutex::scoped_lock lock(to_submit_list_mutex_);
		if(!to_submit_list_.empty())
		{
			return false;
		}
	}
	{
		boost::mutex::scoped_lock lock(delivering_list_mutex_);
		if(!delivered_list_.empty())
		{
			return false;
		}
	}
	{
		boost::mutex::scoped_lock lock(delivered_list_mutex_);
		if(!delivered_list_.empty())
		{
			return false;
		}
	}
	{
		boost::mutex::scoped_lock lock(mo_list_mutex_);
		if(!mo_list_.empty())
		{
			return false;
		}
	}
	{
		boost::mutex::scoped_lock lock(waiting_dlr_timeout_list_mutex_);
		if(!waiting_dlr_timeout_list_.empty())
		{
			return false;
		}
	}
	return true;
}
bool client_data_source::add_smis(bool is_gateway, const std::vector<cb_sm_information>& smis)
{
	boost::mutex::scoped_lock lock(to_submit_list_mutex_);
	for(size_t i = 0; i < smis.size(); i++)
	{
		to_submit_list_.push_back(smis.at(i));
	}
	clients_statistic::get_instance()->sms_received((int)smis.size());
	return true;
}

bool client_data_source::get_to_submit(cb_sm_information& smi)
{
	boost::mutex::scoped_lock lock(to_submit_list_mutex_);
	if(to_submit_list_.empty())
	{
		return false;
	}
	std::list<cb_sm_information>::iterator it = to_submit_list_.begin();
	smi = *it;
	to_submit_list_.erase(it);
	clients_statistic::get_instance()->submit_sms(1);
	return true;
}

void client_data_source::sm_is_delivering(bool is_gateway, cb_sm_information& smi)
{
	boost::mutex::scoped_lock lock(delivering_list_mutex_);
	boost::uint64_t now = (boost::uint64_t)time(NULL);
	unsigned int sn = is_gateway ? smi.sn_by_gateway : smi.sn_by_client;
	std::string message_id = is_gateway ? smi.message_id_of_provider : smi.message_id_of_protocol_server;
	if(is_gateway)
	{
		smi.delivering_time_of_gateway = now;
	}
	else
	{
		smi.delivering_time_of_client = now;
	}
	delivering_list_.push_back(smi);
	logger_log(__FUNCTION__, LEVEL_DEBUG, "smi is delivering, session id: %s, sn:%u, message id:%s, dst:%s, src_addr:%s ", smi.session_id.c_str(), sn, message_id.c_str(), smi.dst_addr.c_str(), smi.src_addr.c_str());
}
bool client_data_source::sm_is_delivered(bool is_gateway, const std::string& message_id, const std::string& error_status, int error_code, bool using_hex_decimal_message_id)
{
	bool found = false;
	cb_sm_information smi;

	std::string real_message_id = message_id;
	if(using_hex_decimal_message_id)
	{
		bool is_number_only = string_is_number_only(message_id);
		if(is_number_only)
		{
			real_message_id = format_decimal_string_to_hex_string(message_id);
		}
		else
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "message id: %s is not a pure decimal string ", message_id.c_str());
		}
		if(real_message_id.length() == 0)
		{
			real_message_id = message_id;
			logger_log(__FUNCTION__, LEVEL_WARNING, "change decimal message id: %s to hex failed", message_id.c_str());
		}
	}
	
	boost::mutex::scoped_lock lock(delivering_list_mutex_);
	if(!delivering_list_.empty())
	{
		std::list<cb_sm_information>::iterator it = delivering_list_.begin();
		for(; it != delivering_list_.end(); it++)
		{
			std::string ack_message_id = is_gateway ? it->message_id_of_provider : it->message_id_of_protocol_server;
			if(compare_string_nocase(ack_message_id, real_message_id) == 0)
			{
				smi = *it;
				delivering_list_.erase(it);
				smi.error_status = error_status;
				smi.error_code = error_code;
				if(is_gateway)
				{
					smi.delivery_time_of_gateway = (boost::uint64_t)time(NULL);
				}
				else
				{
					smi.delivery_time_of_client = (boost::uint64_t)time(NULL);
				}
				
				found = true;
				logger_log(__FUNCTION__, LEVEL_INFO, "message id: %s(session id: %s) is received, error_status: %s, error_code: %d", real_message_id.c_str(), smi.session_id.c_str(), error_status.c_str(), error_code);
				break;
			}
		}
	}
	if(found)
	{
		boost::mutex::scoped_lock lock(delivered_list_mutex_);
		delivered_list_.push_back(smi);
		clients_statistic::get_instance()->a_sm_is_delivered();
		record_dlr_to_db(smi);
	}
	else
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "smi of message id %s(%s) not found", real_message_id.c_str(), message_id.c_str());
	}
	return found;
}
void client_data_source::mo_received(bool is_gateway, const std::string& sysid, unsigned int sn, const std::string& src_addr, const std::string& dst_addr, unsigned char dc_type, const std::string& content)
{
	cb_sm_information smi;
	smi.systemid = sysid;
	if(is_gateway)
	{
		smi.sn_by_provider = sn;
		smi.delivering_time_of_gateway = (boost::uint64_t)time(NULL);
	}
	else
	{
		smi.sn_by_protocol_server = sn;
		smi.delivery_time_of_client = (boost::uint64_t)time(NULL);
	}
	smi.src_addr = src_addr;
	smi.dst_addr = dst_addr;
	smi.datacoding_type = dc_type;
	smi.content = content;
	boost::mutex::scoped_lock lock(mo_list_mutex_);
	mo_list_.push_back(smi);
	clients_statistic::get_instance()->a_mo_received();
	record_mos_to_db(smi);
	logger_log(__FUNCTION__, LEVEL_INFO, "mo received: src:%s, dst:%s, dc:%d, content:%s", src_addr.c_str(),dst_addr.c_str(), (int)dc_type, content.c_str());
}
void client_data_source::get_mos(std::vector<cb_sm_information>& smis)
{
	smis.clear();
	boost::mutex::scoped_lock lock(mo_list_mutex_);
	if(mo_list_.empty())
	{
		return ;
	}
	std::list<cb_sm_information>::iterator it = mo_list_.begin();
	for(; it != mo_list_.end();)
	{
		smis.push_back(*it);
		it = mo_list_.erase(it);
	}
	//logger_log(__FUNCTION__, LEVEL_DEBUG, "get %d mos",(int)smis.size());
	clients_statistic::get_instance()->push_mos((unsigned int)smis.size());
}
void client_data_source::process_waiting_dlr_timeout(bool is_gateway, int waiting_time)
{
	boost::mutex::scoped_lock lock(delivering_list_mutex_);
	std::list<cb_sm_information>::iterator it = delivering_list_.begin();
	for(; it != delivering_list_.end(); )
	{
		boost::uint64_t delivering_time = is_gateway ? it->delivering_time_of_gateway : it->delivering_time_of_client;
		int t = (int)((boost::uint64_t)time(NULL) - delivering_time);
		if(t > waiting_time)
		{
			cb_sm_information smi;
			smi = *it;
			it = delivering_list_.erase(it);
			smi.error_status = DLR_STAT_DLR_TIMEOUT;
			unsigned sn = is_gateway ? smi.sn_by_gateway : smi.sn_by_client;
			logger_log(__FUNCTION__, LEVEL_WARNING, "sn:%u, dst:%s, delivering time: %u(%u), waiting dlr timeout, we will set it be failed", sn, smi.dst_addr.c_str(), (unsigned int)delivering_time, (unsigned int)time(NULL));
			submit_failed(is_gateway, smi);
			
		}
		else
		{
			it++;
		}
	}
}
void client_data_source::get_global_status(bool is_gateway, int to_get, std::vector<cb_sm_information>& smis)
{
	smis.clear();
	boost::mutex::scoped_lock lock(delivered_list_mutex_);
	if(delivered_list_.empty())
	{
		return;
	}
	std::list<cb_sm_information>::iterator it = delivered_list_.begin();
	int count = 0;
	for(; it != delivered_list_.end();)
	{
		smis.push_back(*it);
		it = delivered_list_.erase(it);
		count++;
		if(count >= to_get)
		{
			break;
		}
	}
	logger_log(__FUNCTION__, LEVEL_DEBUG, "get global statuses, total get: %d",(int)smis.size());
	clients_statistic::get_instance()->push_dlrs((unsigned int)smis.size());
}

void client_data_source::submit_failed(bool is_gateway, cb_sm_information& smi)
{
	boost::mutex::scoped_lock lock(delivered_list_mutex_);
	delivered_list_.push_back(smi);
	clients_statistic::get_instance()->submit_failed();
	unsigned int sn = is_gateway ? smi.sn_by_gateway : smi.sn_by_client;
	logger_log(__FUNCTION__, LEVEL_WARNING, "sn: %u, reason: %d", sn, (int)smi.ack_status);
}
int client_data_source::get_delivering_smis_count()
{
	boost::mutex::scoped_lock lock(delivering_list_mutex_);
	return (int)delivering_list_.size();
}
int client_data_source::get_to_submit_smis_count()
{
	boost::mutex::scoped_lock lock(to_submit_list_mutex_);
	return (int)to_submit_list_.size();
}
void client_data_source::get_all_to_submit_smis(std::vector<cb_sm_information>& smis)
{
	boost::mutex::scoped_lock lock(to_submit_list_mutex_);
	std::list<cb_sm_information>::iterator it = to_submit_list_.begin();
	for(; it != to_submit_list_.end(); it++)
	{
		smis.push_back(*it);
	}
	to_submit_list_.clear();
}
