//
//  smppclient.cpp
//  CBClient
//
//  Created by Alex on 4/17/14.
//  Copyright (c) 2014 BiFang technology. All rights reserved.
//
#include "algorithm.hpp"
#include "smppclient.h"
#include <string.h>
#include "smpp_datacoding_toolkit.h"
#include "client_data_source_manager.hpp"
#include "clients_statistic.hpp"
smppclient::smppclient(const smppclient_configer& configer)
{
	configer_ = configer;
	finished_ = false;
	is_protocol_connected_ = false;
}
smppclient::~smppclient()
{
	{
		boost::mutex::scoped_lock lock(waiting_ack_smis_mutex_);
		waiting_ack_smis_.clear();
	}
	{
		boost::mutex::scoped_lock lock(ack_timeout_smis_mutex_);
		ack_timeout_smis_.clear();
	}
}
std::string smppclient::get_unique_id_of_client()
{
	if(configer_.is_gateway_mode())
	{
		return configer_.gateway_id;
	}
	else
	{
		return configer_.sysid;
	}
}
bool smppclient::is_free()
{
	{
		boost::mutex::scoped_lock lock(waiting_ack_smis_mutex_);
		if(!waiting_ack_smis_.empty())
		{
			return false;
		}
	}
	{
		boost::mutex::scoped_lock lock(ack_timeout_smis_mutex_);
		if(!ack_timeout_smis_.empty())
		{
			return false;
		}
	}
	{
		return client_data_source_manager::get_data_source(get_unique_id_of_client())->is_free();
	}
}
bool smppclient::submit_smis(std::vector<cb_sm_information>& smis, const std::string& failed_reason)
{
	for(size_t i = 0; i < smis.size(); i++)
	{
		if(configer_.is_gateway)
		{
			smis.at(i).gateway_id = configer_.gateway_id;
		}
		else
		{
			smis.at(i).systemid = configer_.sysid;
		}
		//smis.at(i).src_addr = configer_.srcaddr;
	}
	if(failed_reason.length() > 0)
	{
		for(size_t i = 0; i < smis.size(); i++)
		{
			smis.at(i).error_code = 0;
			smis.at(i).error_status = failed_reason;
			client_data_source_manager::get_data_source(get_unique_id_of_client())->submit_failed(configer_.is_gateway, smis.at(i));
		}
		return true;
	}
	else
	{
		return client_data_source_manager::get_data_source(get_unique_id_of_client())->add_smis(configer_.is_gateway,smis);
	}
	
}
void smppclient::set_ack(unsigned int sn, const std::string& message_id, ACK_STATUS status, int error_code)
{
	cb_sm_information smi;
	if(get_waiting_ack_smi(sn, smi) || get_ack_timeout_smi(sn, smi))
	{
		if(configer_.is_gateway)
		{
			smi.message_id_of_provider = message_id;
		}
		else
		{
			smi.message_id_of_protocol_server = message_id;
		}
		smi.ack_status = status;
		smi.error_code = error_code;
		if(status == ACK_STATUS_SUCCESSFULLY)
		{
			client_data_source_manager::get_data_source(get_unique_id_of_client())->sm_is_delivering(configer_.is_gateway, smi);
		}
		else
		{
			client_data_source_manager::get_data_source(get_unique_id_of_client())->submit_failed(configer_.is_gateway, smi);
		}
		return ;
	}
	else
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "sn not found: %u", sn);
	}
}
void smppclient::receiver()
{
	while(!finished_)
	{
		memory_buffer_ptr bp;
		if(read_resp(bp))
		{
			process(bp);
		}
		else
		{
			if(is_connected() && is_protocol_connected())
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(10));
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(200));
			}
		}
	}
}
void smppclient::worker()
{
	while(!finished_)
	{
		unsigned int sn = 0;
		cb_sm_information smi;
		boost::uint64_t last_send_enquire_link_time = 0;
		while(!finished_)
		{
			if(!is_connected())
			{
				if(!connect_for(configer_.serverip, configer_.serverport, 1000, configer_.localip, configer_.localport))
				{
					boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
					continue;
				}
			}
			if(!bind(sn))
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
				continue;
			}
			int count = 0;
			while(!finished_)
			{
				count++;
				if(count > 300) //30 seconds
				{
					logger_log(__FUNCTION__, LEVEL_WARNING, "bind timeout");
					break;
				}
				if(!is_protocol_connected())
				{
					boost::this_thread::sleep(boost::posix_time::milliseconds(100));
				}
				else
				{
					break;
				}
			}
			if(!is_protocol_connected())
			{
				disconnect(true);
				break;
			}
			int speed_control_milliseconds_per_sm = (int)(1000 / configer_.max_sms_per_second);
			int used_milliseconds_this_sm = 0;
			while(!finished_)
			{
				boost::posix_time::ptime time_start = boost::posix_time::microsec_clock::local_time();
				process_dlr_timeout_smis();
				process_waiting_ack_smis();
				process_ack_timeout_smis();
				if(tx_able())
				{
					bool get = false;
					if(smi.content.length() == 0)
					{
						bool too_many = too_many_waiting_ack_smis();
						bool is_long_sm_and_not_last = ((smi.udhi_total_parts > 1) && (smi.udhi_total_parts != smi.udhi_part_index));
						if(too_many)
						{
							if(is_long_sm_and_not_last)
							{
								get = client_data_source_manager::get_data_source(get_unique_id_of_client())->get_to_submit(smi);
							}
							else
							{
								boost::this_thread::sleep(boost::posix_time::milliseconds(200));
								continue;
							}
						}
						else
						{
							get = client_data_source_manager::get_data_source(get_unique_id_of_client())->get_to_submit(smi);
						}
					}
					else //means resend last one
					{
						get = true;
					}
					if(get)
					{
						if(submit(smi))
						{
							smi.content = "";
							boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
							boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse = now - time_start;
							used_milliseconds_this_sm = (int)time_elapse.total_milliseconds();
							if(speed_control_milliseconds_per_sm > used_milliseconds_this_sm)
							{
								boost::this_thread::sleep(boost::posix_time::milliseconds(speed_control_milliseconds_per_sm - used_milliseconds_this_sm));
							}
						}
						else
						{
							break;
						}
					}
					else
					{
						boost::this_thread::sleep(boost::posix_time::milliseconds(200));
					}
				}
				else
				{
					boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
				}
				boost::uint64_t now = (boost::uint64_t)time(NULL);
				if((int)(now - last_send_enquire_link_time) > configer_.heatbeat_period)
				{
					if(!enquire_link(sn))
					{
						break;
					}
					last_send_enquire_link_time = now;
				}
			}
		}
	}
}
void smppclient::process(memory_buffer_ptr& bp)
{
	unsigned int cid = Smpp::get_command_id((const Smpp::Uint8 *)bp.get()->data());
	if(cid == Smpp::CommandId::BindTransceiverResp)
	{
		Smpp::BindTransceiverResp resp;
		bool decoded = true;
		try
		{
			resp.decode((const Smpp::Uint8 *)bp.get()->data());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR, "decode failed, reason: %s", e.what());
			decoded = false;
		}
		if(decoded)
		{
			bind_transceiver_resp_received(resp);
		}
	}
	else if(cid == Smpp::CommandId::BindTransmitterResp)
	{
		Smpp::BindTransmitterResp resp;
		bool decoded = true;
		try
		{
			resp.decode((const Smpp::Uint8 *)bp.get()->data());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR, "decode failed, reason: %s", e.what());
			decoded = false;
		}
		if(decoded)
		{
			bind_transmitter_resp_received(resp);
		}
	}
	else if(cid == Smpp::CommandId::BindReceiverResp)
	{
		Smpp::BindReceiverResp resp;
		bool decoded = true;
		try
		{
			resp.decode((const Smpp::Uint8 *)bp.get()->data());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR, "decode failed, reason: %s", e.what());
			decoded = false;
		}
		if(decoded)
		{
			bind_receiver_resp_received(resp);
		}
	}
	else if(cid == Smpp::CommandId::SubmitSmResp)
	{
		Smpp::SubmitSmResp resp;
		bool decoded = true;
		try
		{
			resp.decode((const Smpp::Uint8 *)bp.get()->data());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR, "decode failed, reason: %s", e.what());
			decoded = false;
		}
		if(decoded)
		{
			submitsm_resp_received(resp);
		}
	}
	else if(cid == Smpp::CommandId::DeliverSm)
	{
		Smpp::DeliverSm deliver_sm;
		bool decoded = true;
		try
		{
			deliver_sm.decode((const Smpp::Uint8 *)bp.get()->data());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR, "decode failed, reason: %s", e.what());
			decoded = false;
		}
		if(decoded)
		{
			deliversm_received(deliver_sm);
		}
	}
	else if(cid == Smpp::CommandId::UnbindResp)
	{
		Smpp::UnbindResp resp;
		bool decoded = true;
		try
		{
			resp.decode((const Smpp::Uint8 *)bp.get()->data());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR, "decode failed, reason: %s", e.what());
			decoded = false;
		}
		if(decoded)
		{
			unbind_resp_received(resp);
		}
	}
	else if(cid == Smpp::CommandId::EnquireLinkResp)
	{
		Smpp::EnquireLinkResp resp;
		bool decoded = true;
		try
		{
			resp.decode((const Smpp::Uint8 *)bp.get()->data());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR, "decode failed, reason: %s", e.what());
			decoded = false;
		}
		if(decoded)
		{
			
		}
	}
	else if(cid == Smpp::CommandId::GenericNack)
	{
		Smpp::GenericNack resp;
		bool decoded = true;
		try
		{
			resp.decode((const Smpp::Uint8 *)bp.get()->data());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR, "decode failed, reason: %s", e.what());
			decoded = false;
		}
		if(decoded)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "receive generic ack, status is: 0x%x", (int)resp.command_status());
		}
	}
	else if(cid == Smpp::CommandId::EnquireLink)
	{
		unsigned int sn = Smpp::get_sequence_number((const Smpp::Uint8 *)bp.get()->data());
		process_enquirelink(sn);
	}
	else
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "unknown pdu: 0x%x", (int)cid);
	}
}
void smppclient::process_enquirelink(unsigned int sn)
{
	try
	{
		Smpp::EnquireLinkResp pduResp(Smpp::CommandStatus::ESME_ROK, sn);
		memory_buffer_ptr bpresp(new memory_buffer(pduResp.command_length()));
		bpresp->set_data(0, (void*)pduResp.encode(), pduResp.command_length());
		Smpp::Uint8* d = (Smpp::Uint8*)pduResp.encode();
		if(!write((unsigned char*)d, (size_t)pduResp.command_length()))
		{
			disconnect(true);
		}
	}
	catch (Smpp::Error& e)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is %s.", e.what());
	}
	
}
void smppclient::start()
{
	client_data_source_manager::get_data_source(get_unique_id_of_client());
	receive_thread_.reset(new boost::thread(boost::bind(&smppclient::receiver, this)));
	boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	work_thread_.reset(new boost::thread(boost::bind(&smppclient::worker, this)));
	logger_log(__FUNCTION__, LEVEL_DEBUG, "sysid: %s started.", configer_.sysid.c_str());
}

void smppclient::stop()
{
	unsigned int sn = 0;
	unbind(sn);
	finished_ = true;
	if(work_thread_.get() != NULL)
	{
		work_thread_->join();
		work_thread_.reset();
	}
	if(receive_thread_.get() != NULL)
	{
		receive_thread_->join();
		receive_thread_.reset();
	}
	logger_log(__FUNCTION__, LEVEL_DEBUG, "sysid: %s stopped.", configer_.sysid.c_str());
}
bool smppclient::bind(unsigned int& sn)
{
	if(configer_.bind_mode == BOUND_RX)
	{
		return bind_receiver(sn);
	}
	else if(configer_.bind_mode == BOUND_TRX)
	{
		return bind_transceiver(sn);
	}
	else if(configer_.bind_mode == BOUND_TX)
	{
		return bind_transmitter(sn);
	}
	logger_log(__FUNCTION__, LEVEL_WARNING, "bind type %d is not valid.", (int)configer_.bind_mode);
	return false;
}
bool smppclient::bind_transceiver(unsigned int& sn)
{
	Smpp::BindTransceiver transceiver;
	sn = generate_sn();
	try	
	{
		transceiver.system_id(Smpp::SystemId(configer_.sysid));
		transceiver.password(Smpp::Password(configer_.password));
		transceiver.system_type(configer_.systype);
		transceiver.interface_version(configer_.infver);
		transceiver.sequence_number(sn);
		Smpp::Uint8* d = (Smpp::Uint8*)transceiver.encode();
		bind_sn_ = sn;
		logger_log(__FUNCTION__, LEVEL_DEBUG, "try to bind transceiver to %s:%s...", serverip_.c_str(), serverport_.c_str());
		bool b = write((unsigned char*)d, (size_t)transceiver.command_length());
		if(!b)
		{
			disconnect(true);
		}
		return b;
	}
	catch(Smpp::Error& e)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is %s .", e.what());
		return false;
	}
}
bool smppclient::bind_receiver(unsigned int& sn)
{
	Smpp::BindReceiver receiver;
	sn = generate_sn();
	try
	{
		receiver.system_id(Smpp::SystemId(configer_.sysid));
		receiver.password(Smpp::Password(configer_.password));
		receiver.system_type(configer_.systype);
		receiver.interface_version(configer_.infver);
		receiver.sequence_number(sn);
		Smpp::Uint8* d = (Smpp::Uint8*)receiver.encode();
		bind_sn_ = sn;
		logger_log(__FUNCTION__, LEVEL_DEBUG, "try to bind receiver to %s:%s...", serverip_.c_str(), serverport_.c_str());
		bool b = write((unsigned char*)d, (size_t)receiver.command_length());
		if(!b)
		{
			disconnect(true);
		}
		return b;
	}
	catch(Smpp::Error& e)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is %s .", e.what());
		return false;
	}
}
bool smppclient::bind_transmitter(unsigned int& sn)
{
	Smpp::BindTransmitter transmitter;
	sn = generate_sn();
	try
	{
		transmitter.system_id(Smpp::SystemId(configer_.sysid));
		transmitter.password(Smpp::Password(configer_.password));
		transmitter.system_type(configer_.systype);
		transmitter.interface_version(configer_.infver);
		transmitter.sequence_number(sn);
		Smpp::Uint8* d = (Smpp::Uint8*)transmitter.encode();
		bind_sn_ = sn;
		logger_log(__FUNCTION__, LEVEL_DEBUG, "try to bind transmitter  to %s:%s...", serverip_.c_str(), serverport_.c_str());
		bool b = write((unsigned char*)d, (size_t)transmitter.command_length());
		if(!b)
		{
			disconnect(true);
		}
		return b;
	}
	catch(Smpp::Error& e)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is %s .", e.what());
		return false;
	}
}
bool smppclient::enquire_link(unsigned int& sn)
{
	sn = generate_sn();
	try
	{
		Smpp::EnquireLink enquirelink(sn); // set the sequence number
		Smpp::Uint8* d = (Smpp::Uint8*)enquirelink.encode();
		bool b = write((unsigned char*)d, (size_t)enquirelink.command_length());
		if(!b)
		{
			disconnect(true);
		}
		return b;
	}
	catch(Smpp::Error& e)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is %s .", e.what());
		return false;
	}

}
bool smppclient::submit(cb_sm_information& smi)
{
	Smpp::SubmitSm submit;
	try
	{
		submit.service_type(configer_.servtype);
		
		Smpp::Ton ton(configer_.srcaddr_ton);
		Smpp::Npi npi(configer_.srcaddr_npi);
		submit.source_addr(Smpp::SmeAddress(ton, npi, Smpp::Address(smi.src_addr)));
		
		submit.destination_addr(
								Smpp::SmeAddress(Smpp::Ton(Smpp::Ton::International),
												 Smpp::Npi(Smpp::Npi::E164),
												 Smpp::Address(smi.dst_addr)));
		submit.registered_delivery(0x01);
		unsigned int sn = configer_.is_gateway ? smi.sn_by_gateway : smi.sn_by_client;
		submit.sequence_number(sn);
		memory_buffer_ptr content;
		int content_real_len;
		int content_utf8_len = (int)smi.content.length();
		content.reset(new memory_buffer(content_utf8_len * 4 + 2 + UDHI_HEADER_TOTAL_BYTES));
		int ucs2_content_length = 0;
		unsigned char* ucs2_content;
		int esm_class;
		if(smi.udhi_total_parts > 1)
		{
			unsigned char* p = (unsigned char*)content->data();
			ucs2_content = p + UDHI_HEADER_TOTAL_BYTES;
			*p++ = UDHI_0_BYTES_VALUE;
			*p++ = UDHI_1_BYTES_VALUE;
			*p++ = UDHI_2_BYTES_VALUE;
			*p++ = smi.udhi_reference;
			*p++ = smi.udhi_total_parts;
			*p++ = smi.udhi_part_index;
			esm_class = 0x40;
			submit.esm_class(esm_class);
		}
		else
		{
			esm_class = 0;
			submit.esm_class(esm_class);
			ucs2_content = (unsigned char*)content->data();
		}
		Smpp::DataCoding dc = Smpp::DataCoding::Alphabet;
		if(smpp_datacoding_is_pure_ascii(smi.content.c_str()))
		{
			dc = Smpp::DataCoding::Alphabet;
			memcpy(ucs2_content, smi.content.c_str(), smi.content.length());
			ucs2_content_length = (int)smi.content.length();
		}
		else
		{
			dc = Smpp::DataCoding::UCS2;
			smpp_datacoding_convert_utf8_to_ucs2((unsigned char*)smi.content.c_str(), ucs2_content, ucs2_content_length);
		}
		if(dc != smi.datacoding_type)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "smi.dc is %d, but real is %d. ", (int)smi.datacoding_type, (int)dc);
		}
		submit.data_coding(dc);
		
		if(smi.udhi_total_parts > 1)
		{
			content_real_len = ucs2_content_length + UDHI_HEADER_TOTAL_BYTES;
		}
		else
		{
			content_real_len = ucs2_content_length;
		}
		submit.short_message(reinterpret_cast<const Smpp::Uint8*>(content->data()), content_real_len);
		Smpp::Uint8* d = (Smpp::Uint8*)submit.encode();
		if(configer_.is_gateway)
		{
			smi.submit_time_of_gateway = (boost::uint64_t)time(NULL);
		}
		else
		{
			smi.submit_time_of_client = (boost::uint64_t)time(NULL);
		}
		add_waiting_ack_smi(smi);
		bool submitted = write((unsigned char*)d, (size_t)submit.command_length());
		if(submitted)
		{
			logger_log(__FUNCTION__, LEVEL_DEBUG, "sysid: %s submits smi successfully, sn: %u, dst: %s, session id: %s, content: %s", configer_.sysid.c_str(), sn, smi.dst_addr.c_str(), smi.session_id.c_str(), smi.content.c_str());
			return true;
		}
		else
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "sysid: %s submits a smi failed, sn: %u, dst: %s, session id: %s, content: %s",configer_.sysid.c_str(), sn, smi.dst_addr.c_str(), smi.session_id.c_str(), smi.content.c_str());
			remove_waiting_ack_smi(sn);
			disconnect(true);
			return false;
		}
	}
	catch(Smpp::Error& e)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is %s .", e.what());
		return false;
	}
}
bool smppclient::unbind(unsigned int& sn)
{
	sn = generate_sn();
	try
	{
		Smpp::Unbind unbind(sn); // set the sequence number
		Smpp::Uint8* d = (Smpp::Uint8*)unbind.encode();
		bool b = write((unsigned char*)d, (size_t)unbind.command_length());
		if(!b)
		{
			disconnect(true);
		}
		return b;
	}
	catch(Smpp::Error& e)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is %s .", e.what());
		return false;
	}
	int i = 0;
	while(is_protocol_connected() && is_connected() && i < 100)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(10));
		i++;
	}
	disconnect(false);
}
void smppclient::bind_transceiver_resp_received(const Smpp::BindTransceiverResp& resp)
{
	if(resp.command_status() == Smpp::CommandStatus::ESME_ROK)
	{
		is_protocol_connected_ = true;
		logger_log(__FUNCTION__, LEVEL_DEBUG, "bind transceiver successfully");
	}
	else
	{
		is_protocol_connected_ = false;
		logger_log(__FUNCTION__, LEVEL_WARNING, "bind transceiver failed, reason is: 0x%x", (int)resp.command_status());
	}
}

void smppclient::bind_transmitter_resp_received(const Smpp::BindTransmitterResp& resp)
{
	if(resp.command_status() == Smpp::CommandStatus::ESME_ROK)
	{
		is_protocol_connected_ = true;
		logger_log(__FUNCTION__, LEVEL_DEBUG, "bind transmitter successfully");
	}
	else
	{
		is_protocol_connected_ = false;
		logger_log(__FUNCTION__, LEVEL_WARNING, "bind transmitter failed, reason is: 0x%x", (int)resp.command_status());
	}
}
void smppclient::bind_receiver_resp_received(const Smpp::BindReceiverResp &resp)
{
	if(resp.command_status() == Smpp::CommandStatus::ESME_ROK)
	{
		is_protocol_connected_ = true;
		logger_log(__FUNCTION__, LEVEL_DEBUG, "bind receiver successfully");
	}
	else
	{
		is_protocol_connected_ = false;
		logger_log(__FUNCTION__, LEVEL_WARNING, "bind receiver failed, reason is: 0x%x", (int)resp.command_status());
	}
}
void smppclient::submitsm_resp_received(const Smpp::SubmitSmResp& resp)
{
	Smpp::Uint32 status = resp.command_status();
	Smpp::Uint32 sn = resp.sequence_number();
	
	if(status == Smpp::CommandStatus::ESME_ROK)
	{
		set_ack(sn, resp.message_id(), ACK_STATUS_SUCCESSFULLY, 0);
	}
	else
	{
		logger_log(__FUNCTION__, LEVEL_ERROR, "submit failed, reason is 0x%x .", (int)resp.command_status());
		set_ack(sn, "", ACK_STATUS_FAILED, (int)resp.command_status());
	}
	clients_statistic::get_instance()->an_ack_is_received();
}
void smppclient::unbind_resp_received(const Smpp::UnbindResp& resp)
{
	is_protocol_connected_ = false;
	disconnect(false);
}
/*
 "id:00000002 sub:001 dlvrd:001 submit date:1405311357 done date:1405311357 stat:DELIVRD err:000 text:000"
 */
bool smppclient::parse_deliversm_short_message(const std::string& sm_string, std::string& message_id,
												std::string& status, int& error_code)
{
	const std::string ID = "id";
	const std::string STAT = "stat";
	const std::string ERR = "err";
	const std::string SPLITTER1 = " ";
	const std::string SPLITTER2 = ":";
	std::vector<std::string> vs1;
	split_by_separator(sm_string, SPLITTER1, vs1);
	for(size_t i = 0; i < vs1.size(); i++)
	{
		std::vector<std::string> vs2;
		split_by_separator(vs1.at(i), SPLITTER2, vs2);
		if(vs2.size() == 2)
		{
			if(vs2.at(0).compare(ID) == 0)
			{
				message_id = vs2.at(1);
			}
			else if(vs2.at(0).compare(STAT) == 0)
			{
				status = vs2.at(1);
			}
			else if(vs2.at(0).compare(ERR) == 0)
			{
				std::string err = vs2.at(1);
				error_code = datatype_convert<int>(err);
			}
		}
	}
	return true;
}
std::string smppclient::convert_to_utf8(const Smpp::ShortMessage& sm, Smpp::DataCoding::DataCodingType dc)
{
	std::vector<Smpp::Uint8>::const_iterator it = sm.begin();
	unsigned char* p = (unsigned char*)&*it;
	std::string content;
	if(dc == Smpp::DataCoding::UCS2)
	{
		unsigned short* ucs2_content = (unsigned short*)p;
		int ucs2_content_length = (int)(sm.length() >> 1);
		content = smpp_datacoding_convert_ucs2_to_utf8(ucs2_content, ucs2_content_length);
	}
	else
	{
		content = std::string(it, sm.end());
	}
	return content;
}
void smppclient::deliversm_received(const Smpp::DeliverSm& deliversm)
{
	bool b = true;
	try
	{
		const Smpp::ShortMessage& sm = deliversm.short_message();
		unsigned char esm_class = (Smpp::Uint8)deliversm.esm_class();
		if(esm_class == 0x04)
		{
			std::string message_id;
			std::string error_status;
			int error_code;
			Smpp::DataCoding::DataCodingType dc_type;
			if(configer_.force_dlr_datacoding == -1) //use
			{
				dc_type = deliversm.data_coding().get_type();
			}
			else
			{
				dc_type = (Smpp::DataCoding::DataCodingType)configer_.force_dlr_datacoding;
			}
			Smpp::String sm_string = convert_to_utf8(sm, dc_type);
			logger_log(__FUNCTION__, LEVEL_DEBUG, "received dlr, sn: %d, conten: %s ", (int)deliversm.sequence_number(), sm_string.c_str());
			parse_deliversm_short_message(sm_string, message_id, error_status, error_code);
			client_data_source_manager::get_data_source(get_unique_id_of_client())->sm_is_delivered(configer_.is_gateway, message_id, error_status, error_code, configer_.using_hex_decimal_message_id);
			Smpp::CommandStatus cs(Smpp::CommandStatus::ESME_ROK);
			Smpp::SequenceNumber sn(deliversm.sequence_number());
			Smpp::DeliverSmResp deliversmresp(cs, sn);
			Smpp::Uint8* d = (Smpp::Uint8*)deliversmresp.encode();
			b = write((unsigned char*)d, (size_t)deliversmresp.command_length());
		}
		else
		{
			Smpp::DataCoding::DataCodingType dc_type;
			if(configer_.force_mo_datacoding == -1) //use
			{
				dc_type = deliversm.data_coding().get_type();
			}
			else
			{
				dc_type = (Smpp::DataCoding::DataCodingType)configer_.force_mo_datacoding;
			}
			std::string src_addr = deliversm.source_addr().address();
			std::string dst_addr = deliversm.destination_addr().address();
			std::string content = convert_to_utf8(deliversm.short_message(), dc_type);
			client_data_source_manager::get_data_source(get_unique_id_of_client())->mo_received(configer_.is_gateway,configer_.sysid,deliversm.sequence_number(), src_addr, dst_addr, dc_type, content);
			logger_log(__FUNCTION__, LEVEL_DEBUG, "receive mo, sn: %d, src addr:%s, dst addr:%s, content:%s ", (int)deliversm.sequence_number(), src_addr.c_str(), dst_addr.c_str(), content.c_str());
			Smpp::CommandStatus cs(Smpp::CommandStatus::ESME_ROK);
			Smpp::SequenceNumber sn(deliversm.sequence_number());
			Smpp::DeliverSmResp deliversmresp(cs, sn);
			Smpp::Uint8* d = (Smpp::Uint8*)deliversmresp.encode();
			b = write((unsigned char*)d, (size_t)deliversmresp.command_length());
		}
		if(!b)
		{
			disconnect(true);
		}
	}
	catch(Smpp::Error& e)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is %s .", e.what());
		return ;
	}
}

bool smppclient::tx_able()
{
	return ((configer_.bind_mode == BOUND_TX) || (configer_.bind_mode == BOUND_TRX));
}
bool smppclient::rx_able()
{
	return ((configer_.bind_mode == BOUND_RX) || (configer_.bind_mode == BOUND_TRX));
}

void smppclient::connection_interrupted()
{
	is_protocol_connected_ = false;
}

bool smppclient::get_waiting_ack_smi(unsigned int sn, cb_sm_information& smi)
{
	boost::mutex::scoped_lock lock(waiting_ack_smis_mutex_);
	if(waiting_ack_smis_.empty())
	{
		return false;
	}
	std::list<cb_sm_information>::iterator it = waiting_ack_smis_.begin();
	for(; it != waiting_ack_smis_.end(); it++)
	{
		unsigned sn_tmp = configer_.is_gateway ? it->sn_by_gateway : it->sn_by_client;
		if(sn_tmp == sn)
		{
			smi = *it;
			waiting_ack_smis_.erase(it);
			return true;
		}
	}
	return false;
}
void smppclient::add_waiting_ack_smi(const cb_sm_information& smi)
{
	boost::mutex::scoped_lock lock(waiting_ack_smis_mutex_);
	waiting_ack_smis_.push_back(smi);
}
void smppclient::remove_waiting_ack_smi(unsigned int sn)
{
	boost::mutex::scoped_lock lock(waiting_ack_smis_mutex_);
	if(waiting_ack_smis_.empty())
	{
		return ;
	}
	std::list<cb_sm_information>::iterator it = waiting_ack_smis_.begin();
	for(; it != waiting_ack_smis_.end(); it++)
	{
		unsigned sn_tmp = configer_.is_gateway ? it->sn_by_gateway : it->sn_by_client;
		if(sn_tmp == sn)
		{
			waiting_ack_smis_.erase(it);
			return;
		}
	}
}

bool smppclient::has_waiting_ack_smis()
{
	boost::mutex::scoped_lock lock(waiting_ack_smis_mutex_);
	return (!waiting_ack_smis_.empty());
}
bool smppclient::too_many_waiting_ack_smis()
{
	boost::mutex::scoped_lock lock(waiting_ack_smis_mutex_);
	return (waiting_ack_smis_.size() >= 8);
}
void smppclient::process_waiting_ack_smis()
{
	boost::mutex::scoped_lock lock(waiting_ack_smis_mutex_);
	std::list<cb_sm_information>::iterator it = waiting_ack_smis_.begin();
	for(; it != waiting_ack_smis_.end(); )
	{
		boost::uint64_t submit_time = configer_.is_gateway ? it->submit_time_of_gateway : it->submit_time_of_client;
		int t = (int)((boost::uint64_t)time(NULL) - submit_time);
		if(t > configer_.waiting_ack_timeout)
		{
			cb_sm_information smi;
			smi = *it;
			it = waiting_ack_smis_.erase(it);
			unsigned sn = configer_.is_gateway ? smi.sn_by_gateway : smi.sn_by_client;
			logger_log(__FUNCTION__, LEVEL_WARNING, "sn:%u, dst:%s, submit time: %u(%u), waiting ack timeout, we will try to process it later", sn, smi.dst_addr.c_str(), (unsigned int)submit_time, (unsigned int)time(NULL));
			a_smis_waiting_ack_timeout(smi);
		}
		else
		{
			it++;
		}
	}
}
void smppclient::a_smis_waiting_ack_timeout(const cb_sm_information& smi)
{
	boost::mutex::scoped_lock lock(ack_timeout_smis_mutex_);
	ack_timeout_smis_.push_back(smi);
}
void smppclient::process_ack_timeout_smis()
{
	boost::mutex::scoped_lock lock(ack_timeout_smis_mutex_);
	std::list<cb_sm_information>::iterator it = ack_timeout_smis_.begin();
	for(; it != ack_timeout_smis_.end(); )
	{
		
		boost::uint64_t submit_time = configer_.is_gateway ? it->submit_time_of_gateway : it->submit_time_of_client;
		int t = (int)((boost::uint64_t)time(NULL) - submit_time);
		if(t > configer_.waiting_ack_timeout * 5)
		{
			cb_sm_information smi;
			smi = *it;
			it = ack_timeout_smis_.erase(it);
			smi.error_status = DLR_STAT_SUBMIT_TIMEOUT;
			unsigned sn = configer_.is_gateway ? smi.sn_by_gateway : smi.sn_by_client;
			logger_log(__FUNCTION__, LEVEL_WARNING, "sn:%u, dst:%s, submit time: %u(%u), waiting ack really timeout, we will set it be failed", sn, smi.dst_addr.c_str(), (unsigned int)submit_time, (unsigned int)time(NULL));
			client_data_source_manager::get_data_source(get_unique_id_of_client())->submit_failed(configer_.is_gateway, smi);
			
		}
		else
		{
			it++;
		}
	}
}
void smppclient::process_dlr_timeout_smis()
{
	client_data_source_manager::get_data_source(get_unique_id_of_client())->process_waiting_dlr_timeout(is_gateway_mode(), configer_.waiting_dlr_timeout);
}
bool smppclient::get_ack_timeout_smi(unsigned int sn, cb_sm_information &smi)
{
	boost::mutex::scoped_lock lock(ack_timeout_smis_mutex_);
	if(ack_timeout_smis_.empty())
	{
		return false;
	}
	std::list<cb_sm_information>::iterator it = ack_timeout_smis_.begin();
	for(; it != ack_timeout_smis_.end(); it++)
	{
		unsigned sn_tmp = configer_.is_gateway ? it->sn_by_gateway : it->sn_by_client;
		if(sn_tmp == sn)
		{
			smi = *it;
			ack_timeout_smis_.erase(it);
			logger_log(__FUNCTION__, LEVEL_WARNING, "sn:%u, dst:%s not really timeout", sn_tmp, smi.dst_addr.c_str());
			return true;
		}
	}
	return false;
}
