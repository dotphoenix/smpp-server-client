/*
 *  db_strings.cpp
 *  JWIPTVServer
 *
 *  Created by Zhu Nengjie on 4/25/15.
 *  Copyright 2015 Bifang technology. All rights reserved.
 *
 */
#include "db_strings.hpp"

std::string db_strings::to_submit_smis_table = "to_submit_smis_table";
std::string db_strings::to_delivery_dlrs_table = "to_delivery_dlrs_table";
std::string db_strings::to_delivery_mos_table = "to_delivery_mos_table";
std::string db_strings::gateways_table = "gateways_table";
std::string db_strings::providers_table = "providers_table";
std::string db_strings::company_accounts_table = "company_accounts_table";
std::string db_strings::subaccounts_table = "subaccounts_table";
std::string db_strings::admin_accounts_table = "admin_accounts_table";
std::string db_strings::rate_group_tables = "rate_group_tables";

std::string db_strings::comma = ",";
std::string db_strings::colon = ":";
std::string db_strings::semicolon = ";";
std::string db_strings::space = " ";
std::string db_strings::left_parenthesis = "(";
std::string db_strings::right_parenthesis = ")";
std::string db_strings::primary_key = " PRIMARY KEY ";
std::string db_strings::not_null = " NOT NULL ";
std::string db_strings::auto_increment = " AUTO_INCREMENT ";
std::string db_strings::values = " VALUES ";
std::string db_strings::insert_into = "INSERT INTO ";
std::string db_strings::set = " SET ";
std::string db_strings::create = "CREATE ";
std::string db_strings::database = " DATABASE ";
std::string db_strings::table = " TABLE ";
std::string db_strings::where_clause = " WHERE ";
std::string db_strings::and_operator = " AND ";
std::string db_strings::or_operator = " OR ";
std::string db_strings::if_not_exists = " IF NOT EXISTS ";
std::string db_strings::update = "UPDATE ";
std::string db_strings::drop = "DROP ";
std::string db_strings::delete_from = "DELETE FROM ";
std::string db_strings::equal = " = ";
std::string db_strings::equal_or_greater = " >= ";
std::string db_strings::equal_or_lesser = " <= ";
std::string db_strings::greater = " > ";
std::string db_strings::lesser = " < ";
std::string db_strings::escape_char = "'";
std::string db_strings::select = "SELECT ";
std::string db_strings::star_char = "*";
std::string db_strings::from = " FROM ";
std::string db_strings::limit = " LIMIT ";
std::string db_strings::limit_1 = " LIMIT 1 ";
std::string db_strings::for_update = " FOR UPDATE ";
std::string db_strings::constraint = "CONSTRAINT ";
std::string db_strings::foreign_key = " FOREIGN KEY ";
std::string db_strings::references = "REFERENCES ";

std::string db_strings::varchar_8 = " varchar(8) ";
std::string db_strings::varchar_16 = " varchar(16) ";
std::string db_strings::varchar_32 = " varchar(32) ";
std::string db_strings::varchar_64 = " varchar(64) ";
std::string db_strings::varchar_128 = " varchar(128) ";
std::string db_strings::varchar_256 = " varchar(256) ";
std::string db_strings::varchar_1024 = " varchar(1024) ";
std::string db_strings::varchar_4096 = " varchar(4096) ";
std::string db_strings::varchar_8192 = " varchar(8192) ";
std::string db_strings::uint64 = " bigint unsigned ";
std::string db_strings::uint32 = " int unsigned ";
std::string db_strings::uint16 = " smallint unsigned ";
std::string db_strings::uint8 = " tinyint unsigned ";
std::string db_strings::double_type = " double ";
std::string db_strings::datetime = "datetime";

std::string db_strings::disabled = "disabled";
std::string db_strings::enabled = "enabled";

std::string db_strings::server_id = "server_id";
std::string db_strings::company_account = "company_account";
std::string db_strings::subaccount = "subaccount";
std::string db_strings::state = "state";
std::string db_strings::sn_by_client = "sn_by_client";
std::string db_strings::sn_by_protocol_server = "sn_by_protocol_server";
std::string db_strings::sn_by_gateway = "sn_by_gateway";
std::string db_strings::sn_by_provider = "sn_by_provider";
std::string db_strings::message_id_of_protocol_server = "message_id_of_protocol_server";
std::string db_strings::message_id_of_provider = "message_id_of_provider";
std::string db_strings::dst_addr = "dst_addr";
std::string db_strings::ip = "ip";
std::string db_strings::session_id = "session_id";
std::string db_strings::gateway_id = "gateway_id";
std::string db_strings::src_addr = "src_addr";
std::string db_strings::ack_status = "ack_status_of";
std::string db_strings::submit_time_of_client = "submit_time_of_client";
std::string db_strings::delivering_time_of_client = "delivering_time_of_client";
std::string db_strings::delivery_time_of_client = "delivery_time_of_client";
std::string db_strings::submit_time_of_gateway = "submit_time_of_gateway";
std::string db_strings::delivering_time_of_gateway = "delivering_time_of_gateway";
std::string db_strings::delivery_time_of_gateway = "delivery_time_of_gateway";
std::string db_strings::error_status = "error_status";
std::string db_strings::error_code = "error_code";
std::string db_strings::udhi_reference = "udhi_reference";
std::string db_strings::udhi_total_parts = "udhi_total_parts";
std::string db_strings::udhi_part_index = "udhi_part_index";
std::string db_strings::datacoding_type = "datacoding_type";
std::string db_strings::content = "content";
std::string db_strings::content_hash = "content_hash";
std::string db_strings::charge_mode_of_client = "charge_mode_of_client";
std::string db_strings::price_of_client = "price_of_client";
std::string db_strings::charge_mode_of_gateway = "charge_mode_of_gateway";
std::string db_strings::price_of_gateway = "price_of_gateway";
std::string db_strings::mcc = "mcc";
std::string db_strings::mnc = "mnc";
std::string db_strings::classification = "classification";
std::string db_strings::valid = "valid";
std::string db_strings::protocol_of_client = "protocol_of_client";
std::string db_strings::protocol_of_gateway = "protocol_of_gateway";

