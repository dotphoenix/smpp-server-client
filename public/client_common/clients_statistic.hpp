//
//  clients_statistic.h
//  client_common
//
//  Created by Alex on 6/6/15.
//  Copyright (c) 2015 BiFang technology. All rights reserved.
//

#ifndef __clients_statistic__
#define __clients_statistic__

class clients_statistic
{
public:
	static clients_statistic* get_instance()
	{
		boost::mutex::scoped_lock lock(mutex_);
		if(instance_ == NULL)
		{
			instance_ = new clients_statistic();
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
	clients_statistic(){initialize_statistic();}
	virtual ~clients_statistic(){}
	static clients_statistic* instance_;
	static boost::mutex mutex_;
	
protected:
	volatile boost::uint64_t total_received_;
	volatile boost::uint64_t total_submit_;
	volatile boost::uint64_t total_ack_received_;
	volatile boost::uint64_t total_delivered_;
	volatile boost::uint64_t total_waiting_dlr_timeout_;
	volatile boost::uint64_t total_waiting_ack_timeout_;
	volatile boost::uint64_t total_pushed_dlrs_;
	volatile boost::uint64_t total_mos_;
	volatile boost::uint64_t total_pushed_mos_;
	volatile boost::uint64_t total_submit_failed_;
	boost::posix_time::ptime time_start_;
	void initialize_statistic()
	{
		total_received_ = 0;
		total_submit_ = 0;
		total_ack_received_ = 0;
		total_delivered_ = 0;
		time_start_ = boost::posix_time::microsec_clock::local_time();
		total_waiting_dlr_timeout_ = 0;
		total_waiting_ack_timeout_ = 0;
		total_pushed_dlrs_ = 0;
		total_mos_ = 0;
		total_pushed_mos_ = 0;
		total_submit_failed_ = 0;
	}
public:
	boost::uint64_t total_received()
	{
		boost::mutex::scoped_lock lock(mutex_);
		return total_received_;
	}
	boost::uint64_t total_submit()
	{
		boost::mutex::scoped_lock lock(mutex_);
		return total_submit_;
	}
	boost::uint64_t total_ack_received()
	{
		boost::mutex::scoped_lock lock(mutex_);
		return total_ack_received_;
	}
	boost::uint64_t total_delivered()
	{
		boost::mutex::scoped_lock lock(mutex_);
		return total_delivered_;
	}
	void sms_received(int received)
	{
		boost::mutex::scoped_lock lock(mutex_);
		total_received_ += received;
	}
	void an_ack_is_received()
	{
		boost::mutex::scoped_lock lock(mutex_);
		total_ack_received_++;
	}
	void a_sm_is_delivered()
	{
		boost::mutex::scoped_lock lock(mutex_);
		total_delivered_++;
	}
	void submit_sms(int submit_nb)
	{
		boost::mutex::scoped_lock lock(mutex_);
		total_submit_ += submit_nb;
	}
	void a_sm_waiting_ack_timeout()
	{
		boost::mutex::scoped_lock lock(mutex_);
		total_waiting_ack_timeout_++;
	}
	void a_sm_waiting_dlr_timeout()
	{
		boost::mutex::scoped_lock lock(mutex_);
		total_waiting_dlr_timeout_++;
	}
	void push_dlrs(int nb)
	{
		boost::mutex::scoped_lock lock(mutex_);
		total_pushed_dlrs_ += nb;
	}
	boost::uint64_t total_undeliveried()
	{
		boost::mutex::scoped_lock lock(mutex_);
		return (total_submit_ - total_delivered_);
	}
	void push_mos(int nb)
	{
		boost::mutex::scoped_lock lock(mutex_);
		total_pushed_mos_ += nb;
	}
	void a_mo_received()
	{
		boost::mutex::scoped_lock lock(mutex_);
		total_mos_++;
	}
	void submit_failed()
	{
		boost::mutex::scoped_lock lock(mutex_);
		total_submit_failed_++;
	}
	std::string statistic()
	{
		std::stringstream ss;
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse = now - time_start_;
		double seconds = time_elapse.total_milliseconds() / 1000.0;
		ss<<"total received: "<<total_received_<<", total submit: "<<total_submit_<<", total ack received:"<<total_ack_received_<<", total delivered: "<<total_delivered_;
		if(seconds > 0)
		{
			ss<< ", average sms/second: "<<total_submit_/seconds;
		}
		if(total_waiting_dlr_timeout_ > 0)
		{
			ss<< ", total waiting dlr timeout: "<<total_waiting_dlr_timeout_;
		}
		if(total_waiting_ack_timeout_ > 0)
		{
			ss<< ", total waiting ack timeout: "<<total_waiting_ack_timeout_;
		}
		ss<< ", total pushed dlrs: "<<total_pushed_dlrs_;
		ss<< ", total pushed mos: "<<total_pushed_mos_<<"(total:"<<total_mos_<<")";
		ss<< ", total submit failed sms: "<<total_submit_failed_;
		std::string s = ss.str();
		return s;
	}
};

#endif /* defined(__clients_statistic__) */
