//
//  smppclient.h
//  CBClient
//
//  Created by Alex on 4/17/14.
//  Copyright (c) 2014 BiFang technology. All rights reserved.
//

#ifndef __CBClient__smppclient__
#define __CBClient__smppclient__
#include "memory_buffer.hpp"
#include "logger.hpp"
#include "smpp.hpp"
#include "sync_tcp_client.hpp"
#include "cb_protocol_public.hpp"
#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include "client_data_source_manager.hpp"
#include "client.h"

enum SMPPCLIENT_BIND_MODE
{
	BOUND_UNKNOWN = 0,
	BOUND_TX = 1,
	BOUND_RX = 2,
	BOUND_TRX = 3
};

inline SMPPCLIENT_BIND_MODE get_bind_mode_by_string(const std::string& bt)
{
	if(bt.compare("TX") == 0)
	{
		return BOUND_TX;
	}
	else if(bt.compare("RX") == 0)
	{
		return BOUND_RX;
	}
	else if(bt.compare("TRX") == 0)
	{
		return BOUND_TRX;
	}
	else
	{
		return BOUND_UNKNOWN;
	}
}
struct smppclient_configer: public client_configer
{
	std::string sysid;
	std::string password;
	Smpp::Uint8 srcaddr_ton;
	Smpp::Uint8 srcaddr_npi;
	SMPPCLIENT_BIND_MODE bind_mode;
	Smpp::SystemType systype;
	Smpp::Uint8 infver;
	Smpp::ServiceType servtype;
public:
	smppclient_configer()
	{
		srcaddr_npi = 0;
		srcaddr_ton = 0;
		infver = 0x34;
	}
	std::string get_account(){return sysid;}
	bool is_gateway_mode(){return is_gateway;}
};

class smppclient: public sync_tcp_client, public client
{
public:
	smppclient(const smppclient_configer& configer);
	~smppclient();
	
protected:
	smppclient_configer configer_;
	std::string convert_to_utf8(const Smpp::ShortMessage& sm, Smpp::DataCoding::DataCodingType dc);
	bool finished_;
	void worker();
	void receiver();
	
	boost::shared_ptr<boost::thread> work_thread_;
	boost::shared_ptr<boost::thread> receive_thread_;
	
	unsigned int bind_sn_;
	unsigned int unbind_sn_;

	void process(memory_buffer_ptr& bp);
	void set_ack(unsigned int sn, const std::string& mid, ACK_STATUS status, int error_code);
	
	boost::mutex waiting_ack_smis_mutex_;
	std::list<cb_sm_information> waiting_ack_smis_;
	bool get_waiting_ack_smi(unsigned int sn, cb_sm_information& smi);
	void add_waiting_ack_smi(const cb_sm_information& smi);
	bool has_waiting_ack_smis();
	void remove_waiting_ack_smi(unsigned int sn);
	bool too_many_waiting_ack_smis();
	void process_waiting_ack_smis();
	
	boost::mutex ack_timeout_smis_mutex_;
	std::list<cb_sm_information> ack_timeout_smis_;
	void a_smis_waiting_ack_timeout(const cb_sm_information& smi);
	bool get_ack_timeout_smi(unsigned int sn, cb_sm_information& smi);
	void process_ack_timeout_smis();
	
	void process_dlr_timeout_smis();
protected:
	void bind_transceiver_resp_received(const Smpp::BindTransceiverResp& resp);
	void bind_transmitter_resp_received(const Smpp::BindTransmitterResp& resp);
	void bind_receiver_resp_received(const Smpp::BindReceiverResp& resp);
	void submitsm_resp_received(const Smpp::SubmitSmResp& resp);
	void unbind_resp_received(const Smpp::UnbindResp& resp);
	void deliversm_received(const Smpp::DeliverSm& deliversm);
	bool parse_deliversm_short_message(const std::string& sm_string, std::string& message_id,
									   std::string& status, int& error_code);
	
	// some smpp server will also send enquirelink to client
	void process_enquirelink(unsigned int sn);
	
	bool bind(unsigned int& sn);
	bool bind_transceiver(unsigned int& sn);
	bool bind_transmitter(unsigned int& sn);
	bool bind_receiver(unsigned int& sn);
	//only return true if get successful ack
	bool submit(cb_sm_information& smi);//not const, the smi will be changed
	bool enquire_link(unsigned int& sn);
	bool unbind(unsigned int& sn);
public:

	virtual void start();
	virtual void stop();
	virtual void connection_established(){}
	virtual void connection_interrupted();
	virtual bool tx_able();
	virtual bool rx_able();

	virtual bool submit_smis(std::vector<cb_sm_information>& smis, const std::string& failed_reason);
	virtual std::string get_unique_id_of_client();
	virtual bool is_gateway_mode(){return configer_.is_gateway_mode();}
	virtual std::string get_protocol(){return configer_.protocol;};
	virtual bool is_free();
	virtual void get_global_status(int to_get, std::vector<cb_sm_information>& smis)
	{
		client_data_source_manager::get_data_source(get_unique_id_of_client())->get_global_status(is_gateway_mode(),to_get, smis);
	}
	virtual void get_mos(std::vector<cb_sm_information>& smis)
	{
		client_data_source_manager::get_data_source(get_unique_id_of_client())->get_mos(smis);
	}
	virtual void update_configer(const client_configer* configer)
	{
		configer_ = *((smppclient_configer*)configer);
	}
	virtual int get_delivering_smis_count()
	{
		return client_data_source_manager::get_data_source(get_unique_id_of_client())->get_delivering_smis_count();
	}
	virtual int get_to_submit_smis_count()
	{
		return client_data_source_manager::get_data_source(get_unique_id_of_client())->get_to_submit_smis_count();
	}
	virtual void get_all_to_submit_smis(std::vector<cb_sm_information>& smis)
	{
		client_data_source_manager::get_data_source(get_unique_id_of_client())->get_all_to_submit_smis(smis);
	}

public:
	static const size_t SMPP_HEADERE_LENGTH = 16;
	virtual size_t get_header_length(){return SMPP_HEADERE_LENGTH;}
	virtual size_t get_max_body_length(){return 1024;}
	virtual size_t get_total_length_from_header(unsigned char* data, size_t length)
	{
		if(length < SMPP_HEADERE_LENGTH)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "length = %d is not valid", (int)length);
		}
		Smpp::Uint32 len = Smpp::get_command_length((Smpp::Uint8*)data);
		return (size_t)len;
	}
	
};

#endif /* defined(__CBClient__smppclient__) */
