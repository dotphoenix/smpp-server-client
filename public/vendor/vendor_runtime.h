//
//  vendor_runtime.h
//  SMPPServer
//
//  Created by Alex on 2018/8/24.
//  Copyright © 2018 BiFang technology. All rights reserved.
//

#ifndef vendor_runtime_h
#define vendor_runtime_h
#include "smppserver_configer.h"
#include <string>
#include "smpp.hpp"

struct client_account
{
	std::string systemid;
	std::string pw;
	std::vector <std::string> bind_ips;
	unsigned char state;
	std::string src_addr; //用于上行短信的时候，通过上行号码获取到用户的
};

class vendor_runtime
{
public:
	vendor_runtime(){}
	virtual ~vendor_runtime(){}
protected:
	smppserver_configer configer_;
	boost::mutex cas_mutex_;
	std::vector<client_account> cas_;
	
	bool finished_;
	std::size_t submit_thread_nb_;
	std::vector< boost::shared_ptr<boost::thread> > submit_threads_;
	std::size_t query_dlrs_and_mos_thread_nb_;
	std::vector< boost::shared_ptr<boost::thread> > query_dlrs_and_mos_threads_;
	boost::shared_ptr<boost::thread> query_accounts_thread_;
	void submit_runner()
	{
		while(!finished_)
		{
			if(!sender())
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(100));
			}
		}
	}
	void query_dlrs_and_mos_runner()
	{
		while(!finished_)
		{
			if(!query_dlrs_and_mos())
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(100));
			}
		}
	}
	void query_accounts_runner()
	{
		while(!finished_)
		{
			if(!query_accounts())
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(100));
			}
		}
	}
	virtual bool sender() = 0;
	virtual bool query_dlrs_and_mos() = 0;
	virtual bool query_accounts() = 0;
public:
	void run()
	{
		finished_ = false;
		for(int s = 0; s < submit_thread_nb_; s++)
		{
			boost::shared_ptr<boost::thread>
			thread(new boost::thread(boost::bind(&vendor_runtime::submit_runner, this)));
			submit_threads_.push_back(thread);
		}
		for(int s = 0; s < query_dlrs_and_mos_thread_nb_; s++)
		{
			boost::shared_ptr<boost::thread>
			thread(new boost::thread(boost::bind(&vendor_runtime::query_dlrs_and_mos_runner, this)));
			query_dlrs_and_mos_threads_.push_back(thread);
		}
		query_accounts_thread_.reset((new boost::thread(boost::bind(&vendor_runtime::query_accounts_runner, this))));
	}
	
	void stop()
	{
		finished_ = true;
		// Wait for all threads in the pool to exit.
		for(int s = 0; s < submit_thread_nb_; ++s)
		{
			submit_threads_[s]->join();
		}
		for(int s = 0; s < query_dlrs_and_mos_thread_nb_; ++s)
		{
			query_dlrs_and_mos_threads_[s]->join();
		}
		query_accounts_thread_->join();
	}
	
	virtual void set_configer(const smppserver_configer& configer)
	{
		configer_ = configer;
	}
	Smpp::CommandStatus check_sysid_pw_bindip(const std::string& sysid, const std::string& pwd, const std::string& bind_ip)
	{
		boost::mutex::scoped_lock lock(cas_mutex_);
		for(size_t i = 0; i < cas_.size(); i++)
		{
			const client_account& ci = cas_.at(i);
			if(ci.systemid.compare(sysid) == 0)
			{
				if(ci.pw.compare(pwd) == 0)
				{
					if(ci.bind_ips.size() == 0)
					{
						return Smpp::CommandStatus::ESME_ROK;
					}
					for(size_t j = 0; j < ci.bind_ips.size(); j++)
					{
						if(ci.bind_ips.at(j).compare(bind_ip) == 0)
						{
							return Smpp::CommandStatus::ESME_ROK;
						}
					}
					return  Smpp::CommandStatus::ESME_RINVBINDIP;
				}
				else
				{
					return Smpp::CommandStatus::ESME_RINVPASWD;
				}
			}
		}
		return  Smpp::CommandStatus::ESME_RINVSYSID;
	}
	
	bool get_systemid_by_src_addr(const std::string& src_addr, std::string& systemid)
	{
		boost::mutex::scoped_lock lock(cas_mutex_);
		for(size_t i = 0; i < cas_.size(); i++)
		{
			if(cas_.at(i).src_addr.compare(src_addr) == 0)
			{
				systemid = cas_.at(i).systemid;
				return true;
			}
		}
		return false;
	}
	bool get_src_addr_by_systemid(const std::string& systemid, std::string& src_addr)
	{
		boost::mutex::scoped_lock lock(cas_mutex_);
		for(size_t i = 0; i < cas_.size(); i++)
		{
			if(cas_.at(i).systemid.compare(systemid) == 0)
			{
				src_addr = cas_.at(i).src_addr;
				return true;
			}
		}
		return false;
	}
	bool get_password_by_systemid(const std::string& systemid, std::string& password)
	{
		boost::mutex::scoped_lock lock(cas_mutex_);
		for(size_t i = 0; i < cas_.size(); i++)
		{
			if(cas_.at(i).systemid.compare(systemid) == 0)
			{
				password = cas_.at(i).pw;
				return true;
			}
		}
		return false;
	}
	
	void set_accounts(std::vector<client_account>& cas)
	{
		boost::mutex::scoped_lock lock(cas_mutex_);
		cas_.clear();
		for(int i = 0; i < cas.size(); i++)
		{
			cas_.push_back(cas.at(i));
		}
	}
};

extern boost::shared_ptr<vendor_runtime> vendor_runtime_;
#endif /* vendor_runtime_h */
