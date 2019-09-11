
//
//  smppserver_datasource.cpp
//  SMPPServer
//
//  Created by Alex on 2018/8/24.
//  Copyright © 2018 BiFang technology. All rights reserved.
//

#include "smppserver_datasource.hpp"
#include "smppserver_statistic.h"
#include "vendor_runtime.h"
#include "logger.hpp"

smppserver_datasource* smppserver_datasource::instance_ = NULL;
boost::mutex smppserver_datasource::mutex_;

void smppserver_datasource::smi_received(const cb_sm_information& smi)
{
	cb_sm_information smi_tmp = smi;
	smi_tmp.protocol_of_client = PROTOCOL_SMPP;
	smi_tmp.server_id = configer_.server_id;
	if(configer_.use_buffer_db)
	{
		std::vector<cb_sm_information> smis;
		smis.push_back(smi_tmp);
		smis_buffer_db_->insert_to_submit_smis(smis);
	}
	else
	{
		boost::mutex::scoped_lock lock(received_smis_queue_mutex_);
		received_smis_queue_.push_back(smi_tmp);
	}
	smppserver_statistic::get_instance()->smi_received(smi.systemid);
}

bool smppserver_datasource::get_from_received_smis_queue(cb_sm_information& smi)
{
	if(configer_.use_buffer_db)
	{
		bool need_get_from_db = false;
		{
			boost::mutex::scoped_lock lock(received_smis_queue_mutex_);
			if(received_smis_queue_.empty())
			{
				need_get_from_db = true;
			}
		}
		if(need_get_from_db)
		{
			std::vector<cb_sm_information> smis;
			smis_buffer_db_->get_to_submit_smis(100, smis);
			if(smis.size() > 0)
			{
				boost::mutex::scoped_lock lock(received_smis_queue_mutex_);
				for(size_t i = 0; i < smis.size(); i++)
				{
					received_smis_queue_.push_back(smis.at(i));
				}
			}
		}
	}
	{
		boost::mutex::scoped_lock lock(received_smis_queue_mutex_);
		if(!received_smis_queue_.empty())
		{
			smi = received_smis_queue_.front();
			received_smis_queue_.pop_front();
			return true;
		}
		return false;
	}
}

void smppserver_datasource::add_to_dlr_list(const cb_sm_information &smi)
{
	//if use buffer db, we add to db
	std::vector<cb_sm_information> smis;
	smis.push_back(smi);
	if(configer_.use_buffer_db)
	{
		smis_buffer_db_->insert_to_delivery_dlrs(smis);
	}
	else
	{
		boost::mutex::scoped_lock lock(dlr_list_mutex_);
		dlr_list_.push_back(smi);
	}
	if(configer_.record_mt_mo)
	{
		records_db_->insert_smis(smis);
	}
	
	smppserver_statistic::get_instance()->dlr_generated(smi.systemid);
}

bool smppserver_datasource::peek_dlr(const std::string& sysid, cb_sm_information& smi)
{
	if(configer_.use_buffer_db)
	{
		bool dlr_list_empty = false;
		{
			boost::mutex::scoped_lock lock(mo_list_mutex_);
			if(mo_list_.empty())
			{
				dlr_list_empty = true;
			}
		}
		if(dlr_list_empty)
		{
			std::vector<cb_sm_information> smis;
			smis_buffer_db_->get_to_delivery_dlrs(configer_.server_id, sysid, 100, smis);
			boost::mutex::scoped_lock lock(dlr_list_mutex_);
			for(size_t i = 0; i < smis.size(); i++)
			{
				smis.at(i).state = SMI_STATE_IS_DELIVERIED;
				dlr_list_.push_back(smis.at(i));
			}
		}
	}
	{
		boost::mutex::scoped_lock lock(dlr_list_mutex_);
		if(!dlr_list_.empty())
		{
			std::list<cb_sm_information>::iterator it = dlr_list_.begin();
			for(; it != dlr_list_.end(); it++)
			{
				if((it->systemid.compare(sysid) == 0) && (it->state == SMI_STATE_IS_DELIVERIED))
				{
					it->state = SMI_STATE_IS_PUSHING;
					smi = *it;
					return true;
				}
			}
		}
		return false;
	}
}
bool smppserver_datasource::remove_dlr(unsigned int sn_by_protocol_server)
{
	boost::mutex::scoped_lock lock(dlr_list_mutex_);
	if(!dlr_list_.empty())
	{
		std::list<cb_sm_information>::iterator it = dlr_list_.begin();
		for(; it != dlr_list_.end(); it++)
		{
			if(sn_by_protocol_server != it->sn_by_protocol_server)
			{
				continue;
			}
			cb_sm_information smi = *it;
			dlr_list_.erase(it);
			smppserver_statistic::get_instance()->dlr_pushed(smi.systemid);
			return true;
		}
	}
	return false;
}

//这两个函数未实现数据库存储的功能，如果需要，则必须补上
void smppserver_datasource::add_to_wait_dlr_list(const cb_sm_information& smi)
{
	boost::mutex::scoped_lock lock(wait_dlr_list_mutex_);
	cb_sm_information smi_tmp = smi;
	smi_tmp.state = SMI_STATE_IS_DELIVERING;
	wait_dlr_list_.push_back(smi_tmp);
}

bool smppserver_datasource::get_from_wait_dlr_list(const std::string& message_id, cb_sm_information& smi)
{
	boost::mutex::scoped_lock lock(wait_dlr_list_mutex_);
	std::list<cb_sm_information>::iterator it = wait_dlr_list_.begin();
	for(; it != wait_dlr_list_.end(); it++)
	{
		if((it->message_id_of_protocol_server.compare(message_id) == 0) && (it->state == SMI_STATE_IS_DELIVERING))
		{
			smi = *it;
			wait_dlr_list_.erase(it);
			return true;
		}
	}
	logger_log(__FUNCTION__, LEVEL_WARNING, "invalid dlr, message id: %s", message_id.c_str());
	return false;
}

std::string smppserver_datasource::query_a_message_id_from_wait_dlr_list()
{
	boost::mutex::scoped_lock lock(wait_dlr_list_mutex_);
	std::list<cb_sm_information>::iterator it = wait_dlr_list_.begin();
	for(; it != wait_dlr_list_.end(); it++)
	{
		return it->message_id_of_protocol_server;
	}
	return "";
}

void smppserver_datasource::add_to_mo_list(const cb_sm_information &smi)
{
	std::vector<cb_sm_information> smis;
	smis.push_back(smi);
	if(configer_.use_buffer_db)
	{
		smis_buffer_db_->insert_to_delivery_mos(smis);
	}
	else
	{
		boost::mutex::scoped_lock lock(mo_list_mutex_);
		mo_list_.push_back(smi);
	}
	if(configer_.record_mt_mo)
	{
		records_db_->insert_mos(smis);
	}
	smppserver_statistic::get_instance()->mo_generated(smi.systemid);
}

bool smppserver_datasource::peek_mo(const std::string& sysid, cb_sm_information& smi)
{
	std::string src_addr;
	if(!vendor_runtime_->get_src_addr_by_systemid(sysid, src_addr))
	{
		return false;
	}
	if(configer_.use_buffer_db)
	{
		bool mo_list_empty = false;
		{
			boost::mutex::scoped_lock lock(mo_list_mutex_);
			if(mo_list_.empty())
			{
				mo_list_empty = true;
			}
		}
		if(mo_list_empty)
		{
			std::vector<cb_sm_information> smis;
			smis_buffer_db_->get_to_delivery_mos(configer_.server_id, src_addr, 100, smis);
			boost::mutex::scoped_lock lock(mo_list_mutex_);
			for(size_t i = 0; i < smis.size(); i++)
			{
				smis.at(i).state = SMI_STATE_IS_DELIVERIED;
				mo_list_.push_back(smis.at(i));
			}
		}
		
	}
	{
		boost::mutex::scoped_lock lock(mo_list_mutex_);
		if(!mo_list_.empty())
		{
			std::list<cb_sm_information>::iterator it = mo_list_.begin();
			for(; it != mo_list_.end(); it++)
			{
				if((it->dst_addr.compare(src_addr) == 0) && (it->state == SMI_STATE_IS_DELIVERIED))
				{
					it->state = SMI_STATE_IS_PUSHING;
					smi = *it;
					return true;
				}
			}
		}
		return false;
	}
	
}
bool smppserver_datasource::remove_mo(unsigned int sn_by_protocol_server)
{
	boost::mutex::scoped_lock lock(mo_list_mutex_);
	if(!mo_list_.empty())
	{
		std::list<cb_sm_information>::iterator it = mo_list_.begin();
		for(; it != mo_list_.end(); it++)
		{
			if(sn_by_protocol_server != it->sn_by_protocol_server)
			{
				continue;
			}
			cb_sm_information smi = *it;
			mo_list_.erase(it);
			smppserver_statistic::get_instance()->mo_pushed(smi.systemid);
			return true;
		}
	}
	return false;
}
bool smppserver_datasource::dlrresp_received(unsigned int sn_by_smpp_server)
{
	bool removed = remove_dlr(sn_by_smpp_server);
	if(!removed)
	{
		removed = remove_mo(sn_by_smpp_server);
	}
	if(!removed)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "dlr(or mo) sn: %u not exists.", sn_by_smpp_server);
	}
	return removed;
}

void smppserver_datasource::set_configer(const smppserver_configer& configer)
{
	configer_ = configer;
	if(configer_.use_buffer_db)
	{
		smis_buffer_db_.reset(new smis_db());
		smis_buffer_db_->do_connect(configer_.smis_buffer_db_host_name, configer_.smis_buffer_db_user_name, configer_.smis_buffer_db_password, configer_.smis_buffer_db_db_name);
	}
	if(configer_.record_mt_mo)
	{
		records_db_.reset(new records_db());
		records_db_->do_connect(configer_.records_db_host_name, configer_.records_db_user_name, configer_.records_db_password, configer_.records_db_db_name);
	}
}
