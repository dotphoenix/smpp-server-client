//
//  clients_manager.h
//  client_common
//
//  Created by Alex on 5/5/14.
//  Copyright (c) 2014 BiFang technology. All rights reserved.
//

#ifndef __client_common__clients_manager__
#define __client_common__clients_manager__
#include <string>
#include <boost/thread/mutex.hpp>
#include <list>
#include <boost/thread.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "logger.hpp"
#include "client.h"

class clients_manager
{
public:
	static clients_manager* get_instance()
	{
		boost::mutex::scoped_lock lock(mutex_);
		if(instance_ == NULL)
		{
			instance_ = new clients_manager();
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
	clients_manager();
	virtual ~clients_manager(){}
	static clients_manager* instance_;
	static boost::mutex mutex_;
protected:
	static boost::mutex clients_mutex_;
	std::vector<boost::shared_ptr<client> > clients_;
public:
	// one configer may include more than one smpp clients
	void add_clients(client_configer* configer);
	void add_clients_from_file(const std::string& config_file);
	void stop_all();
	bool is_protocol_connected(const std::string& sysid);
	//numbers_not_valid means the smis will not be submitted, it will processed locally(usually save to
	//database with an error_status of  DLR_STAT_SUBMIT_INVALID_NUMBER
	
	bool submit_smis(std::vector<cb_sm_information>& smis, const std::string& failed_reason);
	void get_global_status(int to_get, std::vector<cb_sm_information>& smis);
	void get_mos(std::vector<cb_sm_information>& smis);
};
#endif /* defined(__client_common__clients_manager__) */
