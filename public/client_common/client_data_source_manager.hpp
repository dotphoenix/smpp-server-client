//
//  data_source_manager.hpp
//  mongoose
//
//  Created by Alex on 5/26/15.
//  Copyright (c) 2015 BiFang technology. All rights reserved.
//

#ifndef __client_data_source_manager_hpp__
#define __client_data_source_manager_hpp__
#include "algorithm.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <boost/thread/mutex.hpp>
#include "client.h"

class client_data_source
{
	friend class client_data_source_manager;
public:
	client_data_source(const std::string& flag)
	{
		flag_ = flag;
	}
	virtual ~client_data_source();
protected:
	boost::mutex to_submit_list_mutex_;
	std::list<cb_sm_information> to_submit_list_;
	
	boost::mutex delivering_list_mutex_;
	std::list<cb_sm_information> delivering_list_; //ack is arrived, to wait DLR
	
	boost::mutex delivered_list_mutex_;
	std::list<cb_sm_information> delivered_list_;
	
	boost::mutex mo_list_mutex_;
	std::list<cb_sm_information> mo_list_;
	
	boost::mutex waiting_dlr_timeout_list_mutex_;
	std::list<cb_sm_information> waiting_dlr_timeout_list_;
	
	std::string flag_;
public:
	bool is_free();
	void sm_is_delivering(bool is_gateway, cb_sm_information& smi);
	bool sm_is_delivered(bool is_gateway, const std::string& message_id, const std::string& error_status, int error_code, bool using_hex_decimal_message_id);
	bool get_to_submit(cb_sm_information& smi);
	void mo_received(bool is_gateway, const std::string& sysid, unsigned int sn, const std::string& src_addr, const std::string& dst_addr, unsigned char dc_type, const std::string& content);
	void get_mos(std::vector<cb_sm_information>& smis);
	void process_waiting_dlr_timeout(bool is_gateway, int waiting_time);
	bool add_smis(bool is_gateway, const std::vector<cb_sm_information>& smis);
	void get_global_status(bool is_gateway, int to_get, std::vector<cb_sm_information>& smis);
	void submit_failed(bool is_gateway, cb_sm_information& smi);
	int get_delivering_smis_count();
	int get_to_submit_smis_count();
	//remove all to submit smis from to_submit_list_ and return them
	void get_all_to_submit_smis(std::vector<cb_sm_information>& smis);
};

typedef boost::shared_ptr<client_data_source> client_data_source_ptr;

class client_data_source_manager
{
protected:
	client_data_source_manager(){}
	virtual ~client_data_source_manager(){}
	
protected:
	static std::list<client_data_source_ptr> cdsps_;
	static boost::mutex cdsps_mutex_;
	static boost::optional<client_data_source_ptr> get_cdsp_no_lock(const std::string& flag)
	{
		boost::optional<client_data_source_ptr> o(boost::none);
		if(!cdsps_.empty())
		{
			std::list<client_data_source_ptr>::iterator it;
			for (it = cdsps_.begin(); it != cdsps_.end(); ++it)
			{
				if(it->get()->flag_.compare(flag) == 0)
				{
					o = *it;
					return o;
				}
			}
		}
		return o;
	}
public:
	static client_data_source_ptr get_data_source(const std::string& flag)
	{
		boost::mutex::scoped_lock lock(cdsps_mutex_);
		boost::optional<client_data_source_ptr> o = get_cdsp_no_lock(flag);
		if(o)
		{
			return o.get();
		}
		else
		{
			client_data_source_ptr cdsp(new client_data_source(flag));
			cdsps_.push_back(cdsp);
			return cdsp;
		}
	}
};

#endif /* defined(__client_data_source_manager_hpp__) */
