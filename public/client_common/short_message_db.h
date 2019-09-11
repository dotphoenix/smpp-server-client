//
//  short_message_db.h
//  ICBCPlatform
//
//  Created by Alex on 11/27/14.
//  Copyright (c) 2014 BiFang technology. All rights reserved.
//

#ifndef __ICBCPlatform__short_message_db__
#define __ICBCPlatform__short_message_db__
#include "sqlite3pp.hpp"
#include <vector>
#include <string>
#include "cb_protocol_public.hpp"
struct sm_db_pair
{
	std::string key;
	std::string value;
	sm_db_pair(){}
	sm_db_pair(const std::string& k, const std::string& v)
	{
		key = k;
		value = v;
	}
};
class short_message_db
{
public:
	short_message_db(const std::string& filename);
	~short_message_db(){}
protected:
	//full path of the db
	std::string filename_;
	
	std::string table_;
	
	std::string item_company_account_;
	std::string item_subaccount_; //subaccount name
	std::string item_session_id_;
	std::string item_sn_;
	std::string item_message_id_;
	std::string item_src_addr_;
	std::string item_dst_addr_;
	std::string item_submit_time_;
	std::string item_delivering_time_;
	std::string item_delivery_time_;
	std::string item_datacoding_;
	std::string item_hash_;
	std::string item_msg_;
	std::string item_error_status_;
	std::string item_error_code_;
	std::string item_udhi_reference_;
	std::string item_udhi_total_parts_;
	std::string item_udhi_part_index_;
	
	bool delete_internal(const std::vector<sm_db_pair>& pairs);
	bool delete_internal(const std::vector< std::vector<sm_db_pair> >& v_pairs);
	bool update_internal(const std::vector<sm_db_pair>& update_decide_pairs, const std::vector<sm_db_pair>& update_pairs);
	
	
	bool read_smi(sqlite3pp::query::iterator& it, cb_sm_information& smi);
public:
	const std::string& filename(){return filename_;}
	bool insert(const std::vector<cb_sm_information>& smis);
	const std::string& get_item_subaccount_name(){return item_subaccount_;}
	const std::string& get_item_sn_name(){return item_sn_;}
	const std::string& get_item_message_id_name(){return item_message_id_;}
	const std::string& get_item_hash_name(){return item_hash_;}
	const std::string& get_item_error_code_name(){return item_error_code_;}
	const std::string& get_item_error_status_name(){return item_error_status_;}
	const std::string& get_item_src_addr_name(){return item_src_addr_;}
	const std::string& get_item_session_id_name(){return item_session_id_;}
	const std::string& get_item_dst_addr_name(){return item_dst_addr_;}
	const std::string& get_item_submit_time_name(){return item_submit_time_;}
	const std::string& get_item_delivering_time_name(){return item_delivering_time_;}
	const std::string& get_item_delivery_time_name(){return item_delivery_time_;}
	
	std::string get_value(const cb_sm_information& smi, const std::string& item_name);
	
	bool delete_items(const std::vector<sm_db_pair>& delete_decide_pairs);
	bool get(std::vector<cb_sm_information>& smis, const std::vector<sm_db_pair> &get_decide_pairs, const std::vector<std::string>& delete_decide_items);
	bool get(int max_get, std::vector<cb_sm_information>& smis, const std::vector<sm_db_pair> &get_decide_pairs, const std::vector<std::string>& delete_decide_items);
	bool read_all(std::vector<cb_sm_information>& smis);
	bool get_submit_time_before(boost::uint64_t submit_time, std::vector<cb_sm_information>& smis, const std::vector<std::string>& delete_decide_items);

	bool fetch_first_and_update(cb_sm_information& smi, const std::vector<sm_db_pair> &fetch_decide_pairs, const std::vector<std::string> &update_decide_items, const std::vector<sm_db_pair> &update_pairs);
	bool update(const std::vector<sm_db_pair> &update_decide_pairs, const std::vector<sm_db_pair> &update_pairs);
	
	bool query_submit_time_between(boost::uint64_t begin_time, boost::uint64_t end_time, std::vector<cb_sm_information>& smis, const std::vector<sm_db_pair> &query_decide_pairs);
};

#endif /* defined(__ICBCPlatform__short_message_db__) */
