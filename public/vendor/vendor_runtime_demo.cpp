//
//  vendor_runtime_demo.cpp
//  SMPPServer
//
//  Created by Alex on 2018/8/24.
//  Copyright © 2018 Bifang technology. All rights reserved.
//

#include "vendor_runtime_demo.hpp"
#include "smppserver_datasource.hpp"
#include "md5.h"
#include "cb_protocol_public.hpp"
#include "smppserver_statistic.h"

vendor_runtime_demo::vendor_runtime_demo()
{
	submit_thread_nb_ = 1;
	query_dlrs_and_mos_thread_nb_ = 1;
	
	initialize_test_data();
}


bool vendor_runtime_demo::sender()
{
	cb_sm_information smi;
	if(smppserver_datasource::get_instance()->get_from_received_smis_queue(smi))
	{
		if(smi.dst_addr.length() == 0)
		{
			logger_log(__FUNCTION__, "dst_addr should not be empty", "systemid %s, dst: %s, content: %s", smi.systemid.c_str(), smi.dst_addr.c_str(), smi.content.c_str());
			return true;
		}
		//#define SIMULATE_DELAY_SOME_DLRS
#ifndef SIMULATE_DELAY_SOME_DLRS
		cb_sm_information smi2 = smi;
		smppserver_datasource::get_instance()->add_to_wait_dlr_list(smi2);
#else
		bool delay_smi = false;
		int dst_last_char = smi.dst_addr.at(smi.dst_addr.length() - 1) ;
		if(dst_last_char == '3' || dst_last_char == '6')
		{
			delay_smi = true;
		}
		if((now - smi.submit_time_of_client >= 8)
		   || ((now - smi.submit_time_of_client >= 2) && !delay_smi)
		   )
		{
			cb_sm_information smi2 = smi;
			smppserver_datasource::get_instance()->add_to_wait_dlr_lst(smi2);
		}
		else
		{
			smi_received(smi);
		}
		return true;
#endif
	}
	return false;
}
bool vendor_runtime_demo::query_dlrs_and_mos()
{
	std::string sysid;
	static time_t last_time_statistic = time(NULL);

	//
	std::string message_id = smppserver_datasource::get_instance()->query_a_message_id_from_wait_dlr_list();
	if(message_id.length() > 0)
	{
		cb_sm_information smi;

		if(smppserver_datasource::get_instance()->get_from_wait_dlr_list(message_id, smi))
		{
			boost::uint64_t now = (boost::uint64_t)time(NULL);
			smi.delivery_time_of_gateway = now;
			if(now % 2 == 0)
			{
				smi.error_status = DLR_STAT_DELIVRD;
				smi.error_code = 0;
			}
			else
			{
				smi.error_status = DLR_STAT_UNDELIV;
				smi.error_code = 36;
			}
			
			smi.sn_by_protocol_server = generate_sn();
			smi.state = SMI_STATE_IS_DELIVERIED;
			smppserver_datasource::get_instance()->add_to_dlr_list(smi);
		}

		if(sysid.length() > 0)
		{
			static time_t last_send_mo = time(NULL);
			time_t now = time(NULL);
			if(now - last_send_mo >= ((rand() % 1) + 1))
			{
				generate_mos(sysid, 5);
				last_send_mo = now;
			}
		}
		{
			time_t now = time(NULL);
			if((int)(now - last_time_statistic) > 60)
			{
				last_time_statistic = now;
				smppserver_statistic::get_instance()->print();
			}
		}
	}
	return false; //总是返回该值，让调用者可以sleep
}

bool vendor_runtime_demo::query_accounts()
{
	return false;
}
void vendor_runtime_demo::generate_mos(const std::string &sysid, int to_generate)
{
	static int total_generated = 0;
	if(total_generated >= configer_.total_generated_mos || sysid.length() == 0)
	{
		return;
	}
	cb_sm_information smi_mo;
#define SUPPORT_UCS2_MO
#ifdef SUPPORT_UCS2_MO
	static int index = 0;
	if(index == 0)
	{
		smi_mo.content = "this is a mo message";
	}
	else if(index == 1)
	{
		smi_mo.content = "这是一条上行消息";
	}
	else if(index == 2)
	{
		smi_mo.content = "這是一條上行消息";
	}
	else
	{
		smi_mo.content = "this is a mo message，这是一条上行消息，這是一條上行消息";
	}
	index++;
	index %= 4;
#else
	smi_mo.content = "this is a mo message";
#endif
	smi_mo.server_id = configer_.server_id;
	smi_mo.protocol_of_gateway = PROTOCOL_SMPP;
	smi_mo.systemid = sysid;
	smi_mo.delivery_time_of_gateway = (boost::uint64_t)time(NULL);
	smi_mo.delivery_time_of_client = smi_mo.delivering_time_of_gateway;
	smi_mo.sn_by_protocol_server = generate_sn();
	smi_mo.sn_by_provider = generate_sn();
	smi_mo.udhi_reference = 0;
	smi_mo.udhi_total_parts = smi_mo.udhi_part_index = 1;
	smi_mo.gateway_id = configer_.server_id + "_AG";
	smi_mo.state = SMI_STATE_IS_DELIVERIED;
	std::string src_addr;
	if(vendor_runtime_->get_src_addr_by_systemid(sysid, src_addr))
	{
		smi_mo.dst_addr = src_addr;
	}
	else
	{
		return;
	}
	generate_hash_md5((const unsigned char*)smi_mo.content.c_str(), (unsigned int)smi_mo.content.length(), smi_mo.content_hash);
	for(int i = 0; i < to_generate; i++)
	{
		smi_mo.src_addr = std::string("8526") + format_uint32_to_lenght((unsigned int)total_generated++, 7);
		smppserver_datasource::get_instance()->add_to_mo_list(smi_mo);
		if(total_generated >= configer_.total_generated_mos)
		{
			break;
		}
	}
}


void vendor_runtime_demo::initialize_test_data()
{
	client_account ci1 = {"WIN01", "123456", {}, 0, "8618866668801"};
	client_account ci2 = {"WIN02", "123456", {}, 0, "8618866668802"};
	client_account ci3 = {"WIN03", "123456", {}, 0, "8618866668803"};
	client_account ci4 = {"WIN04", "123456", {}, 0, "8618866668804"};
	client_account ci5 = {"MAC01", "123456", {}, 0, "8618866668811"};
	client_account ci6 = {"MAC02", "123456", {}, 0, "8618866668812"};
	client_account ci7 = {"MAC03", "123456", {}, 0, "8618866668813"};
	client_account ci8 = {"MAC04", "123456", {}, 0, "8618866668814"};
	client_account ci9 = {"CB001", "123456", {}, 0, "8618866668821"};
	client_account ci10 = {"CB002", "123456", {}, 0, "8618866668822"};
	client_account ci11 = {"CB003", "123456", {}, 0, "8618866668823"};
	client_account ci12 = {"CB004", "123456", {}, 0, "8618866668824"};
	client_account ci13 = {"SMPP1", "123456", {}, 0, "8618866668831"};
	client_account ci14 = {"SMPP2", "123456", {}, 0, "8618866668832"};
	client_account ci15 = {"SMPP3", "123456", {}, 0, "8618866668833"};
	client_account ci16 = {"SMPP4", "123456", {}, 0, "8618866668834"};
	client_account ci17 = {"ICBC01", "123456", {}, 0, "85266668881"};
	client_account ci18 = {"ICBC02", "123456", {}, 0, "85266668882"};
	client_account ci19 = {"ICBC03", "123456", {}, 0, "85266668883"};
	client_account ci20 = {"ICBC04", "123456", {}, 0, "85266668884"};
	{
		boost::mutex::scoped_lock lock(cas_mutex_);
		cas_.push_back(ci1); cas_.push_back(ci2); cas_.push_back(ci3); cas_.push_back(ci4);
		cas_.push_back(ci5); cas_.push_back(ci6); cas_.push_back(ci7); cas_.push_back(ci8);
		cas_.push_back(ci9);cas_.push_back(ci10); cas_.push_back(ci11); cas_.push_back(ci12);
		cas_.push_back(ci13); cas_.push_back(ci14); cas_.push_back(ci15);cas_.push_back(ci16);
		cas_.push_back(ci17); cas_.push_back(ci18); cas_.push_back(ci19); cas_.push_back(ci20);
	}
}
