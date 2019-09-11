//
//  smppserver_manager.h
//  SMPPServer
//
//  Created by Alex on 12/12/14.
//  Copyright (c) 2014 Bifang technology. All rights reserved.
//

#ifndef __SMPPServer__smppserver__
#define __SMPPServer__smppserver__
#include <string>
#include <boost/thread/mutex.hpp>
#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "logger.hpp"
#include "server.hpp"
#include "smpp.hpp"
#include "cb_protocol_public.hpp"
#include <sstream>
#include "smppserver_datasource.hpp"
#include "smppserver_statistic.h"

class smppserver_runtime
{
public:
	static smppserver_runtime* get_instance()
	{
		boost::mutex::scoped_lock lock(mutex_);
		if(instance_ == NULL)
		{
			instance_ = new smppserver_runtime();
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
	smppserver_runtime();
	virtual ~smppserver_runtime(){}
	static smppserver_runtime* instance_;
	static boost::mutex mutex_;
protected:
	server_ptr server_;
	smppserver_configer configer_;
	//client_statistic_ptr
	bool read_configer();
public:
	bool run_smppserver();
	void stop_smppserver();
	bool use_hex_decimal_message_id(){return configer_.use_hex_decimal_message_id;}
protected:
	bool finished_;
	
public:
	bool need_authorize(){return configer_.need_authorize;}
};
#endif /* defined(__SMPPServer__smppserver_manager__) */
