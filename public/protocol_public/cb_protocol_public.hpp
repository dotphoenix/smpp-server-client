#ifndef CB_PROTOCOL_PUBLIC_H
#define CB_PROTOCOL_PUBLIC_H

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string>
/*DELIVRD EXPIRED DELETED UNDELIV ACCEPTD UNKNOWN REJECTD*/
#define DLR_STAT_DELIVRD "DELIVRD"
#define DLR_STAT_EXPIRED "EXPIRED"
#define DLR_STAT_DELETED "DELETED"
#define DLR_STAT_UNDELIV "UNDELIV"
#define DLR_STAT_ACCEPTD "ACCEPTD"
#define DLR_STAT_UNKNOWN "UNKNOWN"
#define DLR_STAT_REJECTD "REJECTD"

#define DLR_STAT_SOCKET_FAILED "CBSKTFD"  //read or write socket failed
#define DLR_STAT_SUBMIT_FAILED "CBSBTFD" //submit failed
#define DLR_STAT_PACKET_FAILED "CBPKTFD" //submit packet failed
#define DLR_STAT_SUBMIT_TIMEOUT "CBSBTTO" //submit timeout
#define DLR_STAT_SUBMIT_INVALID_NUMBER "CBSBINV" //invalid number
#define DLR_STAT_DLR_TIMEOUT "CBDLRTO" //waiting dlr timeout
#define DLR_STAT_NO_SUITABLE_GATEWAY "CBDLRNG" //not get a suitable gateway

#define UDHI_0_BYTES_VALUE 0x05
#define UDHI_1_BYTES_VALUE 0x00
#define UDHI_2_BYTES_VALUE 0x03
#define UDHI_HEADER_TOTAL_BYTES 0x6



#define PROTOCOL_SMPP "SMPP"
#define PROTOCOL_CMPP "CMPP"
#define PROTOCOL_SGIP "SGIP"
#define PROTOCOL_SMGP "SMGP"
#define PROTOCOL_HTTP "HTTP"

enum ACK_STATUS
{
	ACK_STATUS_IDLE = 0,
	ACK_STATUS_WAITING,
	ACK_STATUS_SUCCESSFULLY,
	ACK_STATUS_FAILED,
	ACK_STATUS_TIMEOUT,
	ACK_STATUS_CANCELLED,
	ACK_STATUS_UNKNOWN
};
enum SMI_STATE
{
	SMI_STATE_INIT = 0,
	SMI_STATE_IS_READY,
	SMI_STATE_IS_SUBMITTING, //正在提交给网关，等待ack
	SMI_STATE_IS_DELIVERING, //已经提交给网关(已收到ack),等待dlr
	SMI_STATE_IS_DELIVERIED,//已收到dlr
	SMI_STATE_IS_PUSHING,
	SMI_STATE_IS_PUSHED
};

/*
 This structure is used by ALL projects of CB SMS SYSTEM, each project may only use
 some items of this structure, no need all.
 */
struct cb_sm_information
{
	std::string server_id;
	std::string company_account;
	std::string systemid;
	SMI_STATE state;
	unsigned int sn_by_client;
	unsigned int sn_by_protocol_server;
	unsigned int sn_by_gateway;
	unsigned int sn_by_provider;
	std::string message_id_of_protocol_server;
	std::string message_id_of_provider;
	std::string dst_addr;
	std::string ip;
	std::string session_id;
	std::string gateway_id;
	std::string src_addr;
	ACK_STATUS ack_status;
	boost::uint64_t submit_time_of_client;
	boost::uint64_t delivering_time_of_client;
	boost::uint64_t delivery_time_of_client;
	boost::uint64_t submit_time_of_gateway;
	boost::uint64_t delivering_time_of_gateway;
	boost::uint64_t delivery_time_of_gateway;
	std::string error_status;
	unsigned int error_code;
	unsigned char udhi_reference;
	unsigned char udhi_total_parts;
	unsigned char udhi_part_index;
	unsigned char datacoding_type;
	std::string content;
	std::string content_hash;
	unsigned char charge_mode_of_client;
	double price_of_client;
	unsigned char charge_mode_of_gateway;
	double price_of_gateway;
	std::string mcc;
	std::string mnc;
	unsigned char classification;
	unsigned char valid; // if including sensitive words, it is invalid
	std::string protocol_of_client;
	std::string protocol_of_gateway;

	cb_sm_information()
	{
		state = SMI_STATE_INIT;
		sn_by_client = 0;
		sn_by_protocol_server = 0;
		sn_by_gateway = 0;
		sn_by_provider = 0;
		ack_status = ACK_STATUS_IDLE;
		error_code = 0;
		submit_time_of_client = 0;
		submit_time_of_gateway = 0;
		delivering_time_of_client = 0;
		delivering_time_of_gateway = 0;
		delivery_time_of_client = 0;
		delivery_time_of_gateway = 0;
		udhi_reference = 0;
		udhi_total_parts = 0;
		udhi_part_index = 0;
		datacoding_type = 0;
		charge_mode_of_client = 0;
		charge_mode_of_gateway = 0;
		price_of_client = 0.0;
		price_of_gateway = 0.0;
		classification = 0;
		valid = 0;
	}
};
unsigned int generate_sn();
std::string generate_message_id();
#endif
