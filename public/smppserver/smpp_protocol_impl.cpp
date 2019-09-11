#include "algorithm.hpp"
#include "smpp_protocol_impl.hpp"
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "smpp_pdu_context.hpp"
#include "md5.h"
#include "smpp_datacoding_toolkit.h"
#include "smppserver_runtime.h"
smpp_impl::smpp_impl()
{
	boost::mutex::scoped_lock lock(process_operation_mutex_);
	SMPP_HEADER_LENGTH = 16;
	SMPP_SERVER_SYSID = std::string("SMPP"); //read from config file later
	OTHER_ERROR_TOLERANCE = 3;//read from config file later
	BIND_ERROR_TOLERANCE = 3;//read from config file later
	current_session_state_ = SESSION_OPEN;
	bind_error_count_ = 0;
	other_error_count_ = 0;
	total_qurey_ = 0;
	total_submit_ = 0;
	error_occured_ = false;
}
smpp_impl::~smpp_impl()
{
	logger_log(__FUNCTION__, LEVEL_DEBUG, "%s(%s:%d) leaves from me", sysid_.c_str(), tcp_client_ip_.c_str(), (int)tcp_client_port_);
	clear_resource();
}
bool smpp_impl::sequencenumber_valid_no_lock(memory_buffer_ptr& bp)
{
	Smpp::Uint32 sequencenumber = Smpp::get_sequence_number(bp.get()->data());
	if(!Smpp::SequenceNumber::valid(sequencenumber))
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "%u is not a valid sequence number. ", sequencenumber);
		error_occured_ = true;
		return false;
	}
	return true;
}
void smpp_impl::other_error_occur()
{
	other_error_count_++;
	if(other_error_count_ >= OTHER_ERROR_TOLERANCE)
	{
		error_occured_ = true;
		logger_log(__FUNCTION__, LEVEL_WARNING, "too many time errors, so we set the smpp connecting status to error. ");
	}
}
void smpp_impl::bind_error_occur()
{
	bind_error_count_++;
	if(bind_error_count_ >= BIND_ERROR_TOLERANCE)
	{
		error_occured_ = true;
		logger_log(__FUNCTION__, LEVEL_WARNING, "too many time bind errors, so we set the smpp connecting status to error. ");
	}	
}

Smpp::SequenceNumber smpp_impl::generate_sequence_number()
{
	return Smpp::SequenceNumber(generate_sn());
}
bool smpp_impl::check_pdu_context_valid(Smpp::CommandId& commandid)
{
	return smpp_pdu_context::valid(current_session_state_, commandid);
}

void smpp_impl::process(memory_buffer_ptr& bp)
{
	boost::mutex::scoped_lock lock(process_operation_mutex_);
	if(!sequencenumber_valid_no_lock(bp))
	{
		return;
	}
	Smpp::Uint32 commandid = Smpp::get_command_id(bp.get()->data());
	Smpp::Uint32 sequencenumber = Smpp::get_sequence_number(bp.get()->data());
	Smpp::CommandId cid(commandid);
	Smpp::SequenceNumber sn(sequencenumber);
	bool context_valid = this->check_pdu_context_valid(cid);
	if(!context_valid)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "context invalid: current state: %d, received command id: %u ",
				   (int)current_session_state_, commandid);
		generate_and_send_nack(Smpp::CommandStatus::ESME_RINVCMDID, sn);
		other_error_occur();
		return;
	}
	
	if(commandid == Smpp::CommandId::BindTransceiver)
	{
		Smpp::BindTransceiver transceiver;
		try
		{
			transceiver.decode(bp.get()->data());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "decode failed, receive an invalid BindTransceiver pdu(%s).", e.what());
			generate_and_send_nack(Smpp::CommandStatus::ESME_RINVMSGLEN, sn);
			other_error_occur();
			return ;
		}
		timer_manager_.bind_transceiver_received();
		bind_time_ = boost::posix_time::second_clock::local_time();
		process_bind_transceiver(transceiver);
	}
	else if(commandid == Smpp::CommandId::BindTransmitter)
	{
		Smpp::BindTransmitter transmitter;
		try
		{
			transmitter.decode(bp.get()->data());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "decode failed, receive an invalid BindTransmitter pdu(%s).", e.what());
			generate_and_send_nack(Smpp::CommandStatus::ESME_RINVMSGLEN, sn);
			other_error_occur();
			return ;
		}
		timer_manager_.bind_transmitter_received();
		
		bind_time_ = boost::posix_time::second_clock::local_time();
		process_bind_transmitter(transmitter);
	}
	else if(commandid == Smpp::CommandId::BindReceiver)
	{
		Smpp::BindReceiver receiver;
		try
		{
			receiver.decode(bp.get()->data());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "decode failed, receive an invalid BindReceiver pdu(%s).", e.what());
			generate_and_send_nack(Smpp::CommandStatus::ESME_RINVMSGLEN, sn);
			other_error_occur();
			return ;
		}
		timer_manager_.bind_receiver_received();
		
		bind_time_ = boost::posix_time::second_clock::local_time();
		process_bind_receiver(receiver);
	}
	else if(commandid == Smpp::CommandId::SubmitSm)
	{
		Smpp::SubmitSm submit_sm;
		try
		{
			submit_sm.decode(bp.get()->data());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "decode failed, receive an invalid SubmitSm pdu(%s).", e.what());
			generate_and_send_nack(Smpp::CommandStatus::ESME_RINVMSGLEN, sn);
			other_error_occur();
			return ;
		}
		if(current_session_state_ == SESSION_BOUND_RX)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "bind type is recieve only.");
			generate_and_send_nack(Smpp::CommandStatus::ESME_RUNKNOWN, sn);
			other_error_occur();
			return;
		}
		timer_manager_.submit_sm_received();
//#define TEST_SUBMIT_FAILED
#ifdef TEST_SUBMIT_FAILED
		static int count = 0;
		count++;
		if(count % 5 == 0)
		{
			Smpp::SubmitSmResp pduResp(Smpp::CommandStatus::ESME_RINVNUMDESTS, sn, Smpp::MessageId(""));
			memory_buffer_ptr bpresp(new memory_buffer(pduResp.command_length()));
			memcpy((void*)bpresp.get()->data(), (void*)pduResp.encode(), pduResp.command_length());
			add_to_out_queue(bpresp);
			return;
		}
		
#endif
		
		process_submitsm(submit_sm);
	}
	else if(commandid == Smpp::CommandId::QuerySm)
	{
	}
	else if(commandid == Smpp::CommandId::Unbind)
	{
		
		Smpp::Unbind unbind;
		try
		{
			unbind.decode(bp.get()->data());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "decode failed, receive an invalid Unbind pdu(%s).", e.what());
			generate_and_send_nack(Smpp::CommandStatus::ESME_RINVMSGLEN, sn);
			other_error_occur();
			return ;
		}
		timer_manager_.unbind_received();
		
		bind_time_ = boost::posix_time::second_clock::local_time();
		process_unbind(unbind);
	}
	else if(commandid == Smpp::CommandId::EnquireLink)
	{
		timer_manager_.enquire_link_received();
		process_enquirelink(sn);
	}
	else if(commandid == Smpp::CommandId::DeliverSmResp)
	{
		Smpp::DeliverSmResp dlr_resp;
		try
		{
			dlr_resp.decode(bp.get()->data());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "decode failed, receive an invalid DeliverSmResp pdu(%s).", e.what());
			other_error_occur();
			return ;
		}
		logger_log(__FUNCTION__, LEVEL_DEBUG, "receive DeliverSmResp sn(%u).", dlr_resp.sequence_number());
		Smpp::Uint32 sn = dlr_resp.sequence_number();
		smppserver_datasource::get_instance()->dlrresp_received((unsigned int)sn);
	}
	else
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "receive an invalid operation pdu, command id: 0x%x.",
				   commandid);
		generate_and_send_nack(Smpp::CommandStatus::ESME_RINVCMDID, sn);
		other_error_occur();
		return ;
	}
}
void smpp_impl::generate_and_send_nack(const Smpp::CommandStatus& status, const Smpp::SequenceNumber& sn)
{
	try
	{
		Smpp::GenericNack pduNack(status, sn);
		memory_buffer_ptr bpresp(new memory_buffer(pduNack.command_length()));
		memcpy((void*)bpresp.get()->data(), (void*)pduNack.encode(), pduNack.command_length());
		if(data_source_flag_.length() == 0)
		{
			private_data_source_.add_to_out_queue(bpresp);
		}
		else
		{
			protocol_data_source_manager::get_instance()->add_to_out_queue_A(data_source_flag_, bpresp);
		}
	} catch (Smpp::Error& e)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is: %s.",
				   e.what());
	}
}

void smpp_impl::process_bind_transceiver(const Smpp::BindTransceiver& transceiver)
{	
	Smpp::SystemId sysid = transceiver.system_id();
	Smpp::Password pwd = transceiver.password();
	Smpp::CommandStatus resp_status = Smpp::CommandStatus::ESME_ROK;
	Smpp::SequenceNumber sn = transceiver.sequence_number();
	if(smppserver_runtime::get_instance()->need_authorize())
	{
		resp_status = vendor_runtime_->check_sysid_pw_bindip((Smpp::String)sysid, (Smpp::String)pwd, tcp_client_ip_);
	}
	logger_log(__FUNCTION__, LEVEL_DEBUG, "%s wants to bind transceiver to me, from %s:%d", ((Smpp::String)sysid).c_str(), tcp_client_ip_.c_str(), (int)tcp_client_port_);
	if(resp_status != Smpp::CommandStatus::ESME_ROK)
	{
		try
		{
			Smpp::BindTransceiverResp pduResp(resp_status, sn, Smpp::SystemId(SMPP_SERVER_SYSID));
			memory_buffer_ptr bpresp(new memory_buffer(pduResp.command_length()));
			memcpy((void*)bpresp.get()->data(), (void*)pduResp.encode(), pduResp.command_length());
			private_data_source_.add_to_out_queue(bpresp);
			bind_error_occur();
			logger_log(__FUNCTION__, LEVEL_WARNING, "%s binds to me failed, resons is: 0x%x", ((std::string)sysid).c_str(), (unsigned int)resp_status);
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is: %s.",
					   e.what());
			generate_and_send_nack(Smpp::CommandStatus::ESME_RINVCMDID, sn);
			other_error_occur();
			return;
		}
	}
	else
	{
		try
		{
			Smpp::BindTransceiverResp pduResp(Smpp::CommandStatus::ESME_ROK, transceiver.sequence_number(),
											  Smpp::SystemId(SMPP_SERVER_SYSID));
			memory_buffer_ptr bpresp(new memory_buffer(pduResp.command_length()));
			memcpy((void*)bpresp.get()->data(), (void*)pduResp.encode(), pduResp.command_length());
			data_source_flag_ = (std::string)sysid;
			private_data_source_.add_to_out_queue(bpresp);
			logger_log(__FUNCTION__, LEVEL_DEBUG, "%s binds to me successfully", ((std::string)sysid).c_str());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is: %s.",
					   e.what());
			generate_and_send_nack(Smpp::CommandStatus::ESME_RINVCMDID, sn);
			other_error_occur();
			return;
		}
		sysid_ = (Smpp::String)sysid;
		current_session_state_ = SESSION_BOUND_TRX;
	}
}

void smpp_impl::process_bind_transmitter(const Smpp::BindTransmitter& transmitter)
{
	Smpp::SystemId sysid = transmitter.system_id();
	Smpp::Password pwd = transmitter.password();
	Smpp::CommandStatus resp_status = Smpp::CommandStatus::ESME_ROK;
	Smpp::SequenceNumber sn = transmitter.sequence_number();
	resp_status = vendor_runtime_->check_sysid_pw_bindip((Smpp::String)sysid, (Smpp::String)pwd,tcp_client_ip_);
	logger_log(__FUNCTION__, LEVEL_DEBUG, "%s wants to bind transmitter to me, from %s:%d", ((Smpp::String)sysid).c_str(), tcp_client_ip_.c_str(), (int)tcp_client_port_);
	if(resp_status != Smpp::CommandStatus::ESME_ROK)
	{
		try
		{
			Smpp::BindTransmitterResp pduResp(resp_status, sn, Smpp::SystemId(SMPP_SERVER_SYSID));
			memory_buffer_ptr bpresp(new memory_buffer(pduResp.command_length()));
			memcpy((void*)bpresp.get()->data(), (void*)pduResp.encode(), pduResp.command_length());
			private_data_source_.add_to_out_queue(bpresp);
			bind_error_occur();
						logger_log(__FUNCTION__, LEVEL_WARNING, "%s binds to me failed, resons is: 0x%x", ((std::string)sysid).c_str(), (unsigned int)resp_status);
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is: %s.",
					   e.what());
			generate_and_send_nack(Smpp::CommandStatus::ESME_RINVCMDID, sn);
			other_error_occur();
			return;
		}
	}
	else
	{
		try
		{
			Smpp::BindTransmitterResp pduResp(Smpp::CommandStatus::ESME_ROK, sn,
											  Smpp::SystemId(SMPP_SERVER_SYSID));
			memory_buffer_ptr bpresp(new memory_buffer(pduResp.command_length()));
			memcpy((void*)bpresp.get()->data(), (void*)pduResp.encode(), pduResp.command_length());
			data_source_flag_ = (std::string)sysid;
			private_data_source_.add_to_out_queue(bpresp);
			logger_log(__FUNCTION__, LEVEL_DEBUG, "%s binds to me successfully", ((std::string)sysid).c_str());
		}
		catch (Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is: %s.",
					   e.what());
			generate_and_send_nack(Smpp::CommandStatus::ESME_RINVCMDID, sn);
			other_error_occur();
			return;
		}
		
		sysid_ = (Smpp::String)sysid;
		current_session_state_ = SESSION_BOUND_TX;
	}
}
void smpp_impl::process_bind_receiver(const Smpp::BindReceiver& receiver)
{
	Smpp::SystemId sysid = receiver.system_id();
	Smpp::Password pwd = receiver.password();
	Smpp::CommandStatus resp_status = Smpp::CommandStatus::ESME_ROK;
	Smpp::SequenceNumber sn = receiver.sequence_number();
	resp_status = vendor_runtime_->check_sysid_pw_bindip((Smpp::String)sysid, (Smpp::String)pwd, tcp_client_ip_);
	logger_log(__FUNCTION__, LEVEL_DEBUG, "%s wants to bind receiver to me, from %s:%d", ((Smpp::String)sysid).c_str(), tcp_client_ip_.c_str(), (int)tcp_client_port_);
	if(resp_status != Smpp::CommandStatus::ESME_ROK)
	{
		try
		{
			Smpp::BindReceiverResp pduResp(resp_status, sn, Smpp::SystemId(SMPP_SERVER_SYSID));
			memory_buffer_ptr bpresp(new memory_buffer(pduResp.command_length()));
			memcpy((void*)bpresp.get()->data(), (void*)pduResp.encode(), pduResp.command_length());
			private_data_source_.add_to_out_queue(bpresp);
			bind_error_occur();
			logger_log(__FUNCTION__, LEVEL_WARNING, "%s binds to me failed, resons is: 0x%x", ((std::string)sysid).c_str(), (unsigned int)resp_status);
			return;
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is: %s.",
					   e.what());
			generate_and_send_nack(Smpp::CommandStatus::ESME_RINVCMDID, sn);
			other_error_occur();
			return;
		}
	}
	else
	{
		try
		{
			Smpp::BindReceiverResp pduResp(Smpp::CommandStatus::ESME_ROK, sn,
										   Smpp::SystemId(SMPP_SERVER_SYSID));
			memory_buffer_ptr bpresp(new memory_buffer(pduResp.command_length()));
			memcpy((void*)bpresp.get()->data(), (void*)pduResp.encode(), pduResp.command_length());
			data_source_flag_ = (std::string)sysid;
			private_data_source_.add_to_out_queue(bpresp);
			logger_log(__FUNCTION__, LEVEL_DEBUG, "%s bind to me successfully", ((std::string)sysid).c_str());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is: %s.",
					   e.what());
			generate_and_send_nack(Smpp::CommandStatus::ESME_RINVCMDID, sn);
			other_error_occur();
			return;
		}
		
		sysid_ = (Smpp::String)sysid;
		current_session_state_ = SESSION_BOUND_RX;
	}
}
std::string smpp_impl::convert_to_utf8(int offset, const Smpp::ShortMessage& sm, Smpp::DataCoding::DataCodingType dc)
{
	std::vector<Smpp::Uint8>::const_iterator it = sm.begin();
	unsigned char* p = (unsigned char*)&*it;
	std::string content;
	if(dc == Smpp::DataCoding::UCS2)
	{
		unsigned short* ucs2_content = (unsigned short*)(p + offset);
		int ucs2_content_length = (int)((sm.length() - offset) >> 1);
		content = smpp_datacoding_convert_ucs2_to_utf8(ucs2_content, ucs2_content_length);
	}
	else
	{
		it += offset;
		content = std::string(it, sm.end());
	}
	return content;
}
void smpp_impl::process_submitsm(const Smpp::SubmitSm& submit_sm)
{
	Smpp::MessageId message_id(generate_message_id());
	Smpp::SequenceNumber sn = submit_sm.sequence_number();
	try
	{
		Smpp::SubmitSmResp pduResp(Smpp::CommandStatus::ESME_ROK, sn, message_id);
		memory_buffer_ptr bpresp(new memory_buffer(pduResp.command_length()));
		memcpy((void*)bpresp.get()->data(), (void*)pduResp.encode(), pduResp.command_length());
//		logger_log(__FUNCTION__, LEVEL_DEBUG, "submit from: %s(%s:%d), sn: %d, response message id: %s",sysid_.c_str(), tcp_client_ip_.c_str(), (int)tcp_client_port_, (unsigned int)sn, ((std::string)message_id).c_str());
		if(data_source_flag_.length() == 0)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "data_source_flag_ should not empty here");
		}
		else
		{
			protocol_data_source_manager::get_instance()->add_to_out_queue_A(data_source_flag_, bpresp);
			smppserver_statistic::get_instance()->ack_sent(sysid_);
		}
	}
	catch(Smpp::Error& e)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is %s.", e.what());
		generate_and_send_nack(Smpp::CommandStatus::ESME_RINVMSGLEN, sn);
		other_error_occur();
		return ;
	}
	total_submit_++;
	cb_sm_information smi;
	smi.ip = tcp_client_ip_;
	smi.systemid = sysid_;
	smi.message_id_of_protocol_server = message_id;
	Smpp::ShortMessage sm = submit_sm.short_message();
	const Smpp::DataCoding::DataCodingType& dc_type = submit_sm.data_coding().get_type();
	smi.datacoding_type = (unsigned char)dc_type;
	smi.udhi_reference = 0;
	smi.udhi_total_parts = 1;
	smi.udhi_part_index = 1;
	smi.submit_time_of_client = (boost::uint64_t)time(NULL);
	int offset = 0;
	if(sm.length() > UDHI_HEADER_TOTAL_BYTES)
	{
		std::vector<Smpp::Uint8>::const_iterator it = sm.begin();
		unsigned char c0 = *(it++);
		unsigned char c1 = *(it++);
		unsigned char c2 = *(it++);
		if((c0 == UDHI_0_BYTES_VALUE) && (c1 == UDHI_1_BYTES_VALUE) && (c2 == UDHI_2_BYTES_VALUE))
		{
			smi.udhi_reference = *(it++);
			smi.udhi_total_parts = *(it++);
			smi.udhi_part_index = *(it++);
			offset = UDHI_HEADER_TOTAL_BYTES;
		}
		smi.content = convert_to_utf8(offset, sm, dc_type);
	}
	else
	{
		smi.content = convert_to_utf8(offset, sm, dc_type);
	}
	generate_hash_md5((unsigned char*)smi.content.c_str(), (unsigned int)smi.content.length(), smi.content_hash);
	smi.dst_addr = submit_sm.destination_addr().address();
	smi.src_addr = submit_sm.source_addr().address();
	smi.sn_by_client = (unsigned int)submit_sm.sequence_number();
	smppserver_datasource::get_instance()->smi_received(smi);
	logger_log(__FUNCTION__, LEVEL_DEBUG, "submit from: %s(%s:%d), sn: %d, response message id: %s, content: %s",sysid_.c_str(), tcp_client_ip_.c_str(), (int)tcp_client_port_, (unsigned int)sn, ((std::string)message_id).c_str(), smi.content.c_str());
}
std::string smpp_impl::format_to_AppendixB_time(time_t t)
{
	struct tm* tt = localtime(&t);
	int year = tt->tm_year;
	year += 1900;
	std::string syear = datatype_convert<std::string>(year);
	syear = syear.substr(syear.length() - 2, 2);
	std::stringstream ss;
	ss<<syear<<format_uint32_to_lenght(tt->tm_mon, 2)<<format_uint32_to_lenght(tt->tm_mday, 2)
	<<format_uint32_to_lenght(tt->tm_hour, 2)<<format_uint32_to_lenght(tt->tm_min, 2);
	std::string s = ss.str();
	return s;
}

bool smpp_impl::send_delivery_sm()
{
	cb_sm_information smi;
	bool peek_dlr = smppserver_datasource::get_instance()->peek_dlr(sysid_, smi);
	if(peek_dlr)
	{
		try
		{
			Smpp::DeliverSm pdu;
			pdu.destination_addr(
								 Smpp::SmeAddress(Smpp::Ton(Smpp::Ton::International),
												  Smpp::Npi(Smpp::Npi::E164),
												  Smpp::Address(smi.src_addr)));
			pdu.source_addr(
							Smpp::SmeAddress(Smpp::Ton(Smpp::Ton::International),
											 Smpp::Npi(Smpp::Npi::E164),
											 Smpp::Address(smi.dst_addr)));
			pdu.sequence_number(smi.sn_by_protocol_server);
			pdu.esm_class(0x4);
			pdu.protocol_id(0);
			pdu.priority_flag(0);
			pdu.schedule_delivery_time("");
			pdu.validity_period("");
			pdu.registered_delivery(0);
			pdu.data_coding(Smpp::DataCoding::Alphabet);
			pdu.sm_default_msg_id(0);
			std::stringstream ss;
			std::string message_id = smi.message_id_of_protocol_server;
			if(smppserver_runtime::get_instance()->use_hex_decimal_message_id())
			{
				message_id = format_hex_string_to_decimal_string(message_id, 10);
				//logger_log(__FUNCTION__, LEVEL_DEBUG, "prepare to send dlr to: %s(%s:%d), hex message id: %s, decimal message id: %s", sysid_.c_str(), tcp_client_ip_.c_str(), (int)tcp_client_port_, smi.message_id.c_str(), message_id.c_str());
			}
			smi.delivery_time_of_client = (boost::uint64_t)time(NULL);
			ss<<"id:"<<message_id<<" "
			<<"sub:001"<<" "<<"dlvrd:001"<<" "
			<<"submit date:"<<format_to_AppendixB_time(smi.submit_time_of_client)<<" "
			<<"done date:"<<format_to_AppendixB_time(smi.delivery_time_of_client)<<" "
			<<"stat:"<<smi.error_status<<" "<<"err:"<<format_uint32_to_lenght(smi.error_code, 3)<<" "<<"text:000";
			pdu.short_message(reinterpret_cast<const Smpp::Uint8*>(ss.str().data()),
							  ss.str().length());
			
			Smpp::Uint8* d = (Smpp::Uint8*)pdu.encode();
			memory_buffer_ptr bp;
			bp.reset(new memory_buffer(pdu.command_length()));
			memcpy((void*)bp.get()->data(), d, pdu.command_length());
			if(data_source_flag_.length() == 0)
			{
				logger_log(__FUNCTION__, LEVEL_WARNING, "data_source_flag_ should not empty here");
			}
			else
			{
				protocol_data_source_manager::get_instance()->add_to_out_queue_B(data_source_flag_, bp);
			}
			logger_log(__FUNCTION__, LEVEL_DEBUG, "send dlr to: %s, sn: %d, dst: %s, dlr content: %s", sysid_.c_str(), smi.sn_by_client, smi.dst_addr.c_str(), ss.str().c_str());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is %s.", e.what());
		}
		return true;
	}
	return false;
}

bool smpp_impl::send_mo()
{
	cb_sm_information smi;
	bool peek_mo = smppserver_datasource::get_instance()->peek_mo(sysid_, smi);
	if(peek_mo)
	{
		try
		{
			Smpp::DeliverSm pdu;
			pdu.destination_addr(
								 Smpp::SmeAddress(Smpp::Ton(Smpp::Ton::International),
												  Smpp::Npi(Smpp::Npi::E164),
												  Smpp::Address(smi.dst_addr)));
			pdu.source_addr(
							Smpp::SmeAddress(Smpp::Ton(Smpp::Ton::International),
											 Smpp::Npi(Smpp::Npi::E164),
											 Smpp::Address(smi.src_addr)));
			pdu.sequence_number(smi.sn_by_protocol_server);
			pdu.esm_class(0);
			pdu.protocol_id(0);
			pdu.priority_flag(0);
			pdu.schedule_delivery_time("");
			pdu.validity_period("");
			pdu.registered_delivery(0);
			pdu.sm_default_msg_id(0);
			Smpp::DataCoding dc = Smpp::DataCoding::Alphabet;
			memory_buffer_ptr content;
			int content_utf8_len = (int)smi.content.length();
			content.reset(new memory_buffer(content_utf8_len * 4 + 2 + UDHI_HEADER_TOTAL_BYTES));
			int ucs2_content_length = 0;
			unsigned char* ucs2_content = (unsigned char*)content->data();;
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
			pdu.data_coding(dc);
			pdu.short_message(reinterpret_cast<const Smpp::Uint8*>(content->data()), ucs2_content_length);
			
			Smpp::Uint8* d = (Smpp::Uint8*)pdu.encode();
			memory_buffer_ptr bp;
			bp.reset(new memory_buffer(pdu.command_length()));
			memcpy((void*)bp.get()->data(), d, pdu.command_length());
			if(data_source_flag_.length() == 0)
			{
				logger_log(__FUNCTION__, LEVEL_WARNING, "data_source_flag_ should not empty here");
			}
			else
			{
				protocol_data_source_manager::get_instance()->add_to_out_queue_B(data_source_flag_, bp);
			}
			logger_log(__FUNCTION__, LEVEL_DEBUG, "send mo to: %s, sn: %d, dst: %s, mo content: %s", smi.systemid.c_str(), smi.sn_by_protocol_server, smi.dst_addr.c_str(), smi.content.c_str());
		}
		catch(Smpp::Error& e)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is %s.", e.what());
		}
		return true;
	}
	return false;
}

void smpp_impl::process_enquirelink(const Smpp::SequenceNumber& sn)
{
	try
	{
		Smpp::EnquireLinkResp pduResp(Smpp::CommandStatus::ESME_ROK, sn);
		memory_buffer_ptr bpresp(new memory_buffer(pduResp.command_length()));
		memcpy((void*)bpresp.get()->data(), (void*)pduResp.encode(), pduResp.command_length());
		private_data_source_.add_to_out_queue(bpresp);
	}
	catch (Smpp::Error& e)
	{
		generate_and_send_nack(Smpp::CommandStatus::ESME_RINVMSGLEN, sn);
		other_error_occur();
		logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is %s.", e.what());
	}

}
void smpp_impl::process_unbind(const Smpp::Unbind& unbind)
{
	current_session_state_ = SESSION_CLOSED;
	Smpp::SequenceNumber sn = unbind.sequence_number();
	try
	{
		Smpp::UnbindResp pduResp(Smpp::CommandStatus::ESME_ROK,sn);
		memory_buffer_ptr bpresp(new memory_buffer(pduResp.command_length()));
		memcpy((void*)bpresp.get()->data(), (void*)pduResp.encode(), pduResp.command_length());
		private_data_source_.add_to_out_queue(bpresp);
	}
	catch (Smpp::Error& e)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "generate pdu failed, reason is %s.", e.what());
		generate_and_send_nack(Smpp::CommandStatus::ESME_RINVMSGLEN, sn);
		other_error_occur();
		return ;
	}

}

bool smpp_impl::timer_timeout()
{	
	SMPP_TIMER_EXPIRED_ACTION action;
	return timer_manager_.expired(action);
}

bool smpp_impl::need_send()
{
	if(is_bound())
	{
		send_delivery_sm();
		send_mo();
	}
	bool need = private_data_source_.need_send();
	if(data_source_flag_.length() > 0)
	{
		if(smpp_pdu_context::is_state_bound_txable(current_session_state_))
		{
			need |= protocol_data_source_manager::get_instance()->need_send_A(get_data_source_flag());
		}
		
		if(smpp_pdu_context::is_state_bound_rxable(current_session_state_))
		{
			need |= protocol_data_source_manager::get_instance()->need_send_B(get_data_source_flag());
		}

	}
	return need;
}
bool smpp_impl::need_process()
{
	bool need = private_data_source_.need_process();
	if(data_source_flag_.length() > 0)
	{
		need |= protocol_data_source_manager::get_instance()->need_process(get_data_source_flag());
	}
	return need;
}
int smpp_impl::clear_resource()
{
	private_data_source_.clear_packets();
	if(data_source_flag_.length() > 0)
	{
		//do not clear the packets in protocol_data_source_manager
		//protocol_data_source_manager::get_instance()->clear_packets(data_source_flag_);
	}
	return 0;
}

bool smpp_impl::get_from_out_queue(memory_buffer_ptr& bp)
{
	bool has = private_data_source_.get_from_out_queue(bp);
	if(data_source_flag_.length() == 0)
	{
		return has;
	}
	if(!has)
	{
		if(smpp_pdu_context::is_state_bound_txable(current_session_state_))
		{
			has = protocol_data_source_manager::get_instance()->get_from_out_queue_A(data_source_flag_, bp);
		}
		if(has)
		{
			return has;
		}
		if(smpp_pdu_context::is_state_bound_rxable(current_session_state_))
		{
			has = protocol_data_source_manager::get_instance()->get_from_out_queue_B(data_source_flag_, bp);
		}
	}
	return has;
}
