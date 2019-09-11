//
//  smppserver_statistic.h
//  SMPPServer
//
//  Created by Alex on 2018/8/24.
//  Copyright © 2018 BiFang technology. All rights reserved.
//

#ifndef smppserver_statistic_h
#define smppserver_statistic_h
#include <string>
#include <boost/thread/mutex.hpp>
#include <list>
#include <boost/thread.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <sstream>
#include "logger.hpp"
struct client_statistic
{
	friend class smppserver_statistic;
	client_statistic(const std::string& sysid)
	{
		sysid_ = sysid;
		total_recevied_ = 0;
		total_acks_ = 0;
		total_dlrs_ = 0;
		total_pushed_dlrs_ = 0;
		total_mos_ = 0;
		total_pushed_mos_ = 0;
	}
protected:
	volatile unsigned int total_recevied_;
	volatile unsigned int total_acks_;
	volatile unsigned int total_dlrs_;
	volatile unsigned int total_pushed_dlrs_;
	volatile unsigned int total_mos_;
	volatile unsigned int total_pushed_mos_;
	std::string sysid_;
public:
	void smi_received(){total_recevied_++;}
	void ack_sent(){total_acks_++;}
	void dlr_generated(){total_dlrs_++;}
	void dlr_pushed(){total_pushed_dlrs_++;}
	void mo_generated(){total_mos_++;}
	void mo_pushed(){total_pushed_mos_++;}
	void print()
	{
		std::stringstream ss;
		ss<<"STATISTIC:("<<sysid_<<"): received:"<<total_recevied_
		<<", sent ack:"<<total_acks_
		<<", dlrs:"<<total_dlrs_
		<<", pushed dlrs:"<<total_pushed_dlrs_
		<<", mos:"<<total_mos_
		<<", pushed mos:"<<total_pushed_mos_;
		std::string s = ss.str();
		logger_log(__FUNCTION__, LEVEL_WARNING, "%s", s.c_str());
	}
};
typedef boost::shared_ptr<client_statistic> client_statistic_ptr;
class smppserver_statistic
{
public:
	static smppserver_statistic* get_instance()
	{
		boost::mutex::scoped_lock lock(mutex_);
		if(instance_ == NULL)
		{
			instance_ = new smppserver_statistic();
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
	smppserver_statistic(){}
	virtual ~smppserver_statistic(){}
	static smppserver_statistic* instance_;
	static boost::mutex mutex_;
protected:
	std::list<client_statistic_ptr> clients_;
	boost::mutex clients_mutex_;
	client_statistic_ptr get(const std::string& sysid)
	{
		if(sysid.length() == 0)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "sysid should not be empty");
		}
		boost::mutex::scoped_lock lock(clients_mutex_);
		std::list<client_statistic_ptr>::iterator it = clients_.begin();
		for(; it != clients_.end(); it++)
		{
			if(it->get()->sysid_.compare(sysid) == 0)
			{
				return *it;
			}
		}
		client_statistic_ptr csp(new client_statistic(sysid));
		clients_.push_back(csp);
		return csp;
	}
public:
	void smi_received(const std::string& sysid){get(sysid)->smi_received();}
	void ack_sent(const std::string& sysid){get(sysid)->ack_sent();}
	void dlr_generated(const std::string& sysid){get(sysid)->dlr_generated();}
	void dlr_pushed(const std::string& sysid){get(sysid)->dlr_pushed();}
	void mo_generated(const std::string& sysid){get(sysid)->mo_generated();}
	void mo_pushed(const std::string& sysid){get(sysid)->mo_pushed();}
	void print()
	{
		boost::mutex::scoped_lock lock(clients_mutex_);
		std::list<client_statistic_ptr>::iterator it = clients_.begin();
		for(; it != clients_.end(); it++)
		{
			it->get()->print();
		}
	}
};


#endif /* smppserver_statistic_h */
