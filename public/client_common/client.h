//
//  client.h
//  client_common
//
//  Created by Alex on 4/17/15.
//  Copyright (c) 2015 BiFang technology. All rights reserved.
//

#ifndef __client_common__client__
#define __client_common__client__
#include <string>
#include <vector>
#include "cb_protocol_public.hpp"
#include "client_data_source_manager.hpp"
struct client_configer
{
	std::string protocol;
	std::string serverip;
	std::string serverport;
	std::string localip;
	unsigned short localport;
	bool using_hex_decimal_message_id;
	int force_dlr_datacoding;
	int force_mo_datacoding;
	int max_sms_per_second;
	bool record_mt;
	bool record_mo;
	int heatbeat_period;
	int waiting_ack_timeout;
	int waiting_dlr_timeout;
	bool is_gateway;
	std::string gateway_id;
	client_configer()
	{
		localip = "0.0.0.0";
		localport = 0;
		protocol = PROTOCOL_SMPP;
		using_hex_decimal_message_id = false;
		force_dlr_datacoding = -1;
		force_mo_datacoding = -1;
		max_sms_per_second = 100;
		record_mt = false;
		record_mo = false;
		heatbeat_period = 30;
		waiting_ack_timeout = 60;
		waiting_dlr_timeout = 60 * 60 * 24 * 3;
		is_gateway = false;
	}
public:
	virtual std::string get_account() = 0;
};

class client
{
protected:
	bool is_protocol_connected_;
public:
	virtual void connection_established(){}
	virtual void connection_interrupted() = 0;
	virtual bool tx_able() = 0;
	virtual bool rx_able() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual std::string get_unique_id_of_client() = 0;
	virtual bool is_gateway_mode() = 0;
	virtual std::string get_protocol() = 0;
	virtual bool submit_smis(std::vector<cb_sm_information>& smis, const std::string& failed_reason) = 0;
	virtual bool is_free() = 0; // not working now
	virtual int get_delivering_smis_count() = 0;
	virtual int get_to_submit_smis_count() = 0;
	//remove all to submit smis from to_submit_list_ and return them
	virtual void get_all_to_submit_smis(std::vector<cb_sm_information>& smis) = 0;
public:
	bool is_protocol_connected(){return is_protocol_connected_;}

	virtual void get_global_status(int to_get, std::vector<cb_sm_information>& smis) = 0;
	virtual void get_mos(std::vector<cb_sm_information>& smis) = 0;
	virtual void update_configer(const client_configer* configer) = 0;
};

#endif /* defined(__CBClient__smppclient__) */
