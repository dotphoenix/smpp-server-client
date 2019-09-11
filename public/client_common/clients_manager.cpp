//
//  clients_manager.cpp
//  client_common
//
//  Created by Alex on 5/5/15.
//  Copyright (c) 2015 BiFang technology. All rights reserved.
//
#include "algorithm.hpp"
#include "clients_manager.h"
#include "configer.hpp"
#include "client_data_source_manager.hpp"
#ifdef SMPP_CLIENT
#include "smppclient.h"
#endif

#undef CMPP_CLIENT
#ifdef CMPP_CLIENT
#include "cmppclient.h"
#endif

clients_manager* clients_manager::instance_ = NULL;
boost::mutex clients_manager::mutex_;
boost::mutex clients_manager::clients_mutex_;
clients_manager::clients_manager()
{

}
void clients_manager::add_clients_from_file(const std::string &config_file)
{
	if(boost::filesystem::exists(config_file))
	{
#ifdef SMPP_CLIENT
		//read smpp
		{
			std::vector<smppclient_configer> configers;
			configer conf(config_file);
			std::string sec_name_nb = "smpp_client_nb";
			int clients_nb = conf.get_int(sec_name_nb, "nb", 1);
			for(int i = 1; i <= clients_nb; i++)
			{
				smppclient_configer sc_conf;
				sc_conf.infver = 0x34;
				sc_conf.protocol = PROTOCOL_SMPP;
				std::string sec_name = "smpp_client" + datatype_convert<std::string>(i);
				sc_conf.serverip = conf.get_string(sec_name, "serverip", "");
				sc_conf.serverport = conf.get_string(sec_name, "serverport", "");
				sc_conf.localip = conf.get_string(sec_name, "localip", "0.0.0.0");
				sc_conf.localport = conf.get_int(sec_name, "localport", 0);
				sc_conf.sysid = conf.get_string(sec_name, "sysid", "dt");
				sc_conf.password = conf.get_string(sec_name, "password", "");
				sc_conf.srcaddr_ton = conf.get_int(sec_name, "srcaddr_ton", 0);
				sc_conf.srcaddr_npi = conf.get_int(sec_name, "srcaddr_npi", 0);
				sc_conf.systype = Smpp::SystemType(conf.get_string(sec_name, "systype", ""));
				sc_conf.servtype = Smpp::ServiceType(conf.get_string(sec_name, "servtype", ""));
				std::string bind_mode = conf.get_string(sec_name, "bind_mode", "");
				sc_conf.bind_mode = get_bind_mode_by_string(bind_mode);
				sc_conf.heatbeat_period = conf.get_int(sec_name, "enquire_link_period", 15);
				sc_conf.waiting_ack_timeout = conf.get_int(sec_name, "waiting_ack_timeout", 15);
				sc_conf.waiting_dlr_timeout = conf.get_int(sec_name, "waiting_dlr_timeout", 3600 * 60 * 24 * 3);//3 days
				sc_conf.using_hex_decimal_message_id = (1 == conf.get_int(sec_name, "using_hex_decimal_message_id", 0));
				sc_conf.force_dlr_datacoding = conf.get_int(sec_name, "force_dlr_datacoding", -1);
				sc_conf.force_mo_datacoding = conf.get_int(sec_name, "force_mo_datacoding", -1);
				sc_conf.max_sms_per_second = conf.get_int(sec_name, "max_sms_per_second", 2000);
				sc_conf.is_gateway = (1 == conf.get_int(sec_name, "is_gateway", 0));

				if(sc_conf.bind_mode != BOUND_UNKNOWN)
				{
					configers.push_back(sc_conf);
				}
				else
				{
					logger_log(__FUNCTION__, LEVEL_WARNING, "bind type %s is invalid", bind_mode.c_str());
				}
			}
			for(size_t i = 0; i < configers.size(); i++)
			{
				add_clients(&(configers.at(i)));
			}
		}
#endif
#ifdef CMPP_CLIENT
		//read cmpp
		{
			std::vector<cmppclient_configer> configers;
			configer conf(config_file);
			std::string sec_name_nb = "cmpp_client_nb";
			int clients_nb = conf.get_int(sec_name_nb, "nb", 1);
			for(int i = 1; i <= clients_nb; i++)
			{
				cmppclient_configer sc_conf;
				sc_conf.version = Cmpp::Version::V20;
				sc_conf.protocol = PROTOCOL_CMPP;
				std::string sec_name = "cmpp_client" + datatype_convert<std::string>(i);
				sc_conf.serverip = conf.get_string(sec_name, "serverip", "");
				sc_conf.serverport = conf.get_string(sec_name, "serverport", "");
				sc_conf.localip = conf.get_string(sec_name, "localip", "0.0.0.0");
				sc_conf.localport = conf.get_int(sec_name, "localport", 0);
				sc_conf.source_address = conf.get_string(sec_name, "source_addr", "");
				sc_conf.shared_secret = conf.get_string(sec_name, "shared_secret", "");
				sc_conf.src_id = conf.get_string(sec_name, "src_id", "");
				sc_conf.heatbeat_period = conf.get_int(sec_name, "enquire_link_period", 15);
				sc_conf.waiting_ack_timeout = conf.get_int(sec_name, "waiting_ack_timeout", 15);
				sc_conf.waiting_dlr_timeout = conf.get_int(sec_name, "waiting_dlr_timeout", 3600 * 60 * 24 * 3);//3 days
				sc_conf.using_hex_decimal_message_id = (1 == conf.get_int(sec_name, "using_hex_decimal_message_id", 0));
				sc_conf.force_dlr_datacoding = conf.get_int(sec_name, "force_dlr_datacoding", -1);
				sc_conf.force_mo_datacoding = conf.get_int(sec_name, "force_mo_datacoding", -1);
				sc_conf.max_sms_per_second = conf.get_int(sec_name, "max_sms_per_second", 2000);
				sc_conf.is_gateway = (1 == conf.get_int(sec_name, "is_gateway", 0));
				configers.push_back(sc_conf);
			}
			for(size_t i = 0; i < configers.size(); i++)
			{
				add_clients(&(configers.at(i)));
			}
		}
#endif
	}
	else
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "%s not exist", config_file.c_str());
	}
	

	
}
void clients_manager::add_clients(client_configer* configer)
{
	boost::mutex::scoped_lock lock(clients_mutex_);
#ifdef SMPP_CLIENT
	if(configer->protocol == PROTOCOL_SMPP)
	{
		smppclient_configer* conf = (smppclient_configer*)configer;
		boost::shared_ptr<client> sc(new smppclient(*conf));
		clients_.push_back(sc);
		sc->start();
	}
#endif
#ifdef CMPP_CLIENT
	if(configer->protocol == PROTOCOL_CMPP)
	{
		cmppclient_configer* conf = (cmppclient_configer*)configer;
		boost::shared_ptr<client> sc(new cmppclient(*conf));
		clients_.push_back(sc);
		sc->start();
	}
#endif
}

bool clients_manager::submit_smis(std::vector<cb_sm_information> &smis, const std::string& failed_reason)
{
	static int last_index = 0;
	boost::mutex::scoped_lock lock(clients_mutex_);
	if(!clients_.empty())
	{
		last_index++;
		int begin_index = (last_index) % clients_.size();
		int index = begin_index;
		while(true)
		{
			if(clients_.at(index)->tx_able())
			{
				clients_.at(index)->submit_smis(smis, failed_reason);
				begin_index = index;
				return true;
			}
			else
			{
				index++;
				index %= clients_.size();
				if(index == begin_index)
				{
					return false;
				}
			}
		}
		return false;
	}
	return false;
}
void clients_manager::get_global_status(int to_get, std::vector<cb_sm_information> &smis)
{
	boost::mutex::scoped_lock lock(clients_mutex_);
	int remain_to_get = to_get;
	for(size_t i = 0; i < clients_.size(); i++)
	{
		std::vector<cb_sm_information> smis_tmp;
		clients_.at(i)->get_global_status(remain_to_get, smis_tmp);
		
		for(size_t j = 0; j < smis_tmp.size(); j++)
		{
			smis.push_back(smis_tmp.at(j));
		}
		if((int)smis.size() >= to_get)
		{
			break;
		}
		remain_to_get -= (int)smis_tmp.size();
	}
}

void clients_manager::get_mos(std::vector<cb_sm_information> &smis)
{
	boost::mutex::scoped_lock lock(clients_mutex_);
	for(size_t i = 0; i < clients_.size(); i++)
	{
		std::vector<cb_sm_information> smis_tmp;
		clients_.at(i)->get_mos(smis_tmp);
		for(size_t j = 0; j < smis_tmp.size(); j++)
		{
			smis.push_back(smis_tmp.at(j));
		}
	}
}

void clients_manager::stop_all()
{
	boost::mutex::scoped_lock lock(clients_mutex_);
	for(size_t i = 0; i < clients_.size(); i++)
	{
		clients_.at(i)->stop();
	}
	clients_.clear();
}

bool clients_manager::is_protocol_connected(const std::string& sysid)
{
	if(sysid.length() > 0)
	{
		boost::mutex::scoped_lock lock(clients_mutex_);
		for(size_t i = 0; i < clients_.size(); i++)
		{
			if(clients_.at(i)->get_unique_id_of_client().compare(sysid) == 0)
			{
				return clients_.at(i)->is_protocol_connected();
			}
		}
		return false;
	}
	else
	{
		boost::mutex::scoped_lock lock(clients_mutex_);
		for(size_t i = 0; i < clients_.size(); i++)
		{
			if(clients_.at(i)->is_protocol_connected())
			{
				return true;
			}
		}
		return false;
	}
}



