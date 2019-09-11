//
//  smppserver_datasource.hpp
//  SMPPServer
//
//  Created by Alex on 2018/8/24.
//  Copyright © 2018 BiFang technology. All rights reserved.
//

#ifndef smppserver_datasource_hpp
#define smppserver_datasource_hpp
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <list>
#include <vector>
#include <deque>
#include <stdio.h>
#include <string>
#include "smis_db.hpp"
#include "records_db.hpp"
#include "smppserver_configer.h"

class smppserver_datasource
{
public:
	static smppserver_datasource* get_instance()
	{
		boost::mutex::scoped_lock lock(mutex_);
		if(instance_ == NULL)
		{
			instance_ = new smppserver_datasource();
		}
		return instance_;
	}
	static void free_instance()
	{
		boost::mutex::scoped_lock lock(mutex_);
		if(instance_ != NULL)
		{
			delete instance_;
			instance_ = NULL;
		}
	}
protected:
	smppserver_datasource(){}
	virtual ~smppserver_datasource(){}
	static smppserver_datasource* instance_;
	static boost::mutex mutex_;
	
protected:
	std::deque<cb_sm_information> received_smis_queue_;
	boost::mutex received_smis_queue_mutex_;
	
	std::list<cb_sm_information> wait_dlr_list_;
	boost::mutex wait_dlr_list_mutex_;
	
	std::list<cb_sm_information> dlr_list_;
	boost::mutex dlr_list_mutex_;
	
	std::list<cb_sm_information> mo_list_;
	boost::mutex mo_list_mutex_;
	
	boost::shared_ptr<smis_db> smis_buffer_db_;
	boost::shared_ptr<records_db> records_db_;
	
	smppserver_configer configer_;
public:
	void smi_received(const cb_sm_information& smi);
	bool get_from_received_smis_queue(cb_sm_information& smi);
	
	void add_to_wait_dlr_list(const cb_sm_information& smi);
	bool get_from_wait_dlr_list(const std::string& message_id, cb_sm_information& smi);
	
	void add_to_dlr_list(const cb_sm_information& smi);
	void add_to_mo_list(const cb_sm_information& smi);
	
	bool peek_dlr(const std::string& sysid, cb_sm_information& smi);
	bool remove_dlr(unsigned int sn_by_protocol_server);
	bool peek_mo(const std::string& dst_addr, cb_sm_information& smi);
	bool remove_mo(unsigned int sn_by_protocol_server);
	bool dlrresp_received(unsigned int sn_by_smpp_server);
	void set_configer(const smppserver_configer& configer);
	
public:
	std::string query_a_message_id_from_wait_dlr_list();
};

typedef boost::shared_ptr<smppserver_datasource> smppserver_datasource_ptr;
#endif /* smppserver_datasource_hpp */
