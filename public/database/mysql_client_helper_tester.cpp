/*
 *  mysql_client_helper_tester.cpp
 *  JWIPTVServer
 *
 *  Created by Zhu Nengjie on 5/5/15.
 *  Copyright 2015 Bifang technology. All rights reserved.
 *
 */
#include "algorithm.hpp"
#include "smis_db.hpp"
#include "md5.h"

void mysql_client_helper_performance_tester();
void mysql_client_helper_tester();
static unsigned int begin_sn = (unsigned int)time(NULL);
static void insert_test(unsigned int total_test)
{
	char msg[1024];
	{
		std::vector<cb_sm_information> smis;
		boost::shared_ptr<smis_db> db(new smis_db());
		db->do_connect("localhost", "root", "alex1234", "smis_buffer_db");
		WORK_START
		for(unsigned int i = 0; i < total_test; i++)
		{
			cb_sm_information smi;
			smi.server_id = "SMPP1";
			smi.company_account = "Bifang";
			smi.systemid = "PM0001";
			smi.state = SMI_STATE_INIT;
			smi.sn_by_client = begin_sn++;
			smi.sn_by_protocol_server = 0;
			smi.sn_by_gateway = 0;
			smi.sn_by_provider = 0;
			smi.message_id_of_protocol_server = "0F8934B5";
			smi.message_id_of_provider = "0F8934BD";
			smi.dst_addr = "8618689898989";
			smi.ip = "192.168.1.99";
			smi.session_id = "0F389DE";
			smi.gateway_id = "GW001";
			smi.src_addr = "106899999999";
			smi.ack_status = ACK_STATUS_IDLE;
			smi.submit_time_of_client = time(NULL);
			smi.delivering_time_of_client = 0;
			smi.delivery_time_of_client = 0;
			smi.submit_time_of_gateway = 0;
			smi.delivering_time_of_gateway = 0;
			smi.delivery_time_of_gateway = 0;
			smi.error_status = DLR_STAT_DELIVRD;
			smi.error_code = 0;
			smi.udhi_reference = 0x90;
			smi.udhi_total_parts = 2;
			smi.udhi_part_index = i % 2;
			smi.datacoding_type = 8;
			//smi.content = "这是一条用于测试的短信，在这条短信里面有中文有英文有数字，还有签名【这是签名】";
			smi.content = "hhh";
			smi.content_hash = "0987654321abcde88";
			smi.charge_mode_of_client = 0;
			smi.price_of_client = 0.19;
			smi.charge_mode_of_gateway = 0;
			smi.price_of_gateway = 0.09;
			smi.mcc = "460";
			smi.mnc = "01";
			smi.classification = 0x9;
			smi.valid = 0x0;
			smi.protocol_of_client = "SMPP";
			smi.protocol_of_gateway = "CMPP";
			smis.push_back(smi);
		}
		db->insert_to_submit_smis(smis);
		sprintf(msg, "insert %d smis in auto commit mode", total_test);
		WORK_FINISH(msg)
	}
}

static void get_test(int total)
{
	std::vector<cb_sm_information> smis;
	{
		boost::shared_ptr<smis_db> db(new smis_db());
		db->do_connect("localhost", "root", "alex1234", "smis_buffer_db");
		WORK_START
		db->peek_smis(total, smis);
		db->delete_smis(smis);
		char msg[128];
		sprintf(msg, "select and delete %d smis ", (int)smis.size());
		WORK_FINISH(msg)
	}
}

void mysql_client_helper_performance_tester()
{
	insert_test(10000);
	get_test(100);
	get_test(1000);
	boost::this_thread::sleep(boost::posix_time::milliseconds(5 * 1000));
	insert_test(10000);
	get_test(100);
	get_test(1000);
	boost::this_thread::sleep(boost::posix_time::milliseconds(5 * 1000));
	insert_test(10000);
	get_test(100);
	get_test(1000);
	get_test(30000);
}

