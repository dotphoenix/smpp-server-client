/*
 *  db_strings.hpp
 *
 *
 *  Created by Zhu Nengjie on 5/25/15.
 *  Copyright 2015 Bifang technology. All rights reserved.
 *
 */
#ifndef MYSQL_DB_STRINGS_H
#define MYSQL_DB_STRINGS_H
#include <string>

class db_strings
{
public:
	static std::string to_submit_smis_table;
	static std::string to_delivery_dlrs_table;
	static std::string to_delivery_mos_table;
	static std::string gateways_table;
	static std::string providers_table;
	static std::string company_accounts_table;
	static std::string subaccounts_table;
	static std::string admin_accounts_table;
	static std::string rate_group_tables;
	
	static std::string comma;
	static std::string colon;
	static std::string semicolon;
	static std::string space;
	static std::string left_parenthesis;
	static std::string right_parenthesis;
	static std::string primary_key;
	static std::string not_null;
	static std::string auto_increment;
	static std::string values;
	static std::string insert_into;
	static std::string set;
	static std::string create;
	static std::string database;
	static std::string table;
	static std::string where_clause;
	static std::string and_operator;
	static std::string or_operator;
	static std::string if_not_exists;
	static std::string update;
	static std::string drop;
	static std::string delete_from;
	static std::string equal;
	static std::string equal_or_greater;
	static std::string equal_or_lesser;
	static std::string greater;
	static std::string lesser;
	static std::string escape_char;
	static std::string select;
	static std::string star_char;
	static std::string from;
	static std::string limit;
	static std::string limit_1;
	static std::string for_update;
	static std::string constraint;
	static std::string foreign_key;
	static std::string references;
	
	static std::string varchar_8;
	static std::string varchar_16;
	static std::string varchar_32;
	static std::string varchar_64;
	static std::string varchar_128;
	static std::string varchar_256;
	static std::string varchar_1024;
	static std::string varchar_4096;
	static std::string varchar_8192;
	static std::string uint64;
	static std::string uint32;
	static std::string uint16;
	static std::string uint8;
	static std::string double_type;
	static std::string datetime;
	
	static std::string disabled;
	static std::string enabled;
	
	static std::string server_id;
	static std::string company_account;
	static std::string subaccount;
	static std::string  state;
	static std::string sn_by_client;
	static std::string sn_by_protocol_server;
	static std::string sn_by_gateway;
	static std::string sn_by_provider;
	static std::string message_id_of_protocol_server;
	static std::string message_id_of_provider;
	static std::string dst_addr;
	static std::string ip;
	static std::string session_id;
	static std::string gateway_id;
	static std::string src_addr;
	static std::string ack_status;
	static std::string submit_time_of_client;
	static std::string delivering_time_of_client;
	static std::string delivery_time_of_client;
	static std::string submit_time_of_gateway;
	static std::string delivering_time_of_gateway;
	static std::string delivery_time_of_gateway;
	static std::string error_status;
	static std::string error_code;
	static std::string udhi_reference;
	static std::string udhi_total_parts;
	static std::string udhi_part_index;
	static std::string datacoding_type;
	static std::string content;
	static std::string content_hash;
	static std::string charge_mode_of_client;
	static std::string price_of_client;
	static std::string charge_mode_of_gateway;
	static std::string price_of_gateway;
	static std::string mcc;
	static std::string mnc;
	static std::string classification;
	static std::string valid; // if including sensitive words, it is invalid
	static std::string protocol_of_client;
	static std::string protocol_of_gateway;
};

#endif
