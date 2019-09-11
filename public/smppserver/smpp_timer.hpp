#ifndef SMPP_TIMER_H
#define SMPP_TIMER_H
#include <vector>
#include <boost/thread/mutex.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <sstream>
#include "smpp.hpp"
#include "logger.hpp"
/*
SMPP Protocol Specification v3.4

Timer                       |   Action on  expiration                                            |    Description


session_init_timer      

The networkconnection should  be terminated.          

This timer specifies the time lapse allowed between a network connection being  established and a 
bind_transmitter or bind_receiver request being sent to the SMSC.This timer should be active on the 
SMSC.

enquire_link_timer     

An enquire_link request should be initiated.     

This timer specifies the time lapse allowed between operations after which an SMPP entity 
should interrogate whether it's peer still has an active session.This timer may be active on 
either communicating SMPP entity (i.e. SMSC or ESME).



inactivity_timer           

The SMPP session should be dropped.     

This timer specifies the maximum time lapse allowed between transactions, after which period of
inactivity, an SMPP entity may assume that the session is no longer active. This timer may be active 
on either communicating SMPP entity (i.e. SMSC or ESME).



response_timer     

The entity which originated the SMPP Request may  assume that Request has not been processed and 
 should take the appropriate action for the particular  SMPP operation in question.

This timer specifies the time lapse allowed between an SMPP request and the corresponding SMPP response. 
This timer may be active on either communicating SMPP entity (i.e. SMSC or ESME).								  
*/


enum SMPP_TIMER_TYPE
{
	TIMER_INIT,
	TIMER_ENQUIRE_LINK,
	TIMER_INACTIVITY,
	TIMER_RESPONSE
};

enum SMPP_TIMER_EXPIRED_ACTION
{
	DROP_SESSION,
	OTHER
};

class smpp_timer
{
public:
	enum STATUS
	{
		NOT_BEGIN,//created but not begin
		BEGIN, //has began
		UPDATE, //has updated
		END //has ended
	};
protected:
	//unit is milliseconds
	boost::uint64_t duration_;
	boost::posix_time::ptime begin_time_;
	boost::posix_time::ptime update_time_;
	boost::posix_time::ptime end_time_;
	SMPP_TIMER_TYPE timer_type_;
	STATUS status_;
	boost::mutex mutex_;
	std::string uid_; //the id identifies the unique timer
public:
	//
	virtual bool expired() = 0;
	//the operation after timer expired
	virtual SMPP_TIMER_EXPIRED_ACTION process() = 0;
public:
	void set_duration(int duration)
	{
		duration_ = duration;
	}
	virtual void begin()
	{
		boost::mutex::scoped_lock lock(mutex_);
		begin_time_ = boost::posix_time::microsec_clock::local_time();
		status_ = BEGIN;
	}
	virtual void update()
	{
		boost::mutex::scoped_lock lock(mutex_);
		update_time_ = boost::posix_time::microsec_clock::local_time();
		status_ = UPDATE;
	}
	virtual void end()
	{
		boost::mutex::scoped_lock lock(mutex_);
		update_time_ = boost::posix_time::microsec_clock::local_time();
		status_ = END;
	}
	virtual bool get_begin_time(boost::posix_time::ptime& time)
	{
		boost::mutex::scoped_lock lock(mutex_);
		if(status_ != NOT_BEGIN)
		{
			time = begin_time_;
			return true;
		}
		return false;
	}
	virtual bool get_update_time(boost::posix_time::ptime& time)
	{
		boost::mutex::scoped_lock lock(mutex_);
		if(status_ < UPDATE)
		{
			time = update_time_;
			return true;
		}
		return false;
	}
	virtual bool get_end_time(boost::posix_time::ptime& time)
	{
		boost::mutex::scoped_lock lock(mutex_);
		if(status_ == END)
		{
			time = end_time_;
			return true;
		}
		return false;
	}
	virtual std::string get_uid()
	{
		return uid_;
	}

	virtual void set_uid(const std::string& uid)
	{
		uid_ = uid;
	}

	SMPP_TIMER_TYPE get_timer_type()
	{
		return timer_type_;
	}
	
	STATUS get_status()
	{
		return status_;
	}
};
/*
 this time begins at the time the connection established, end at the time the bind
 received; if we do not receive the bind in the 'duration' , this timer expired.
 
the operation of expire:disconnect the connection
*/
class session_init_timer: public smpp_timer
{
public:
	session_init_timer()
	{
		//default 10 seconds
		duration_ = 10 * 1000;
		timer_type_ = TIMER_INIT;
		status_ = NOT_BEGIN;
		uid_ = "session_init_timer";
	}
public:
	virtual bool expired()
	{
		if((status_ == END) || (status_ == NOT_BEGIN))
		{
			return false;
		}
		else
		{
			boost::posix_time::millisec_posix_time_system_config::time_duration_type duration;
			duration = boost::posix_time::microsec_clock::local_time() - begin_time_;
			boost::uint64_t past = duration.total_milliseconds();
			return (past  > duration_);
		}
	}
	virtual SMPP_TIMER_EXPIRED_ACTION process(){return DROP_SESSION;};
};
typedef boost::shared_ptr<session_init_timer> session_init_timer_ptr;
/*

 this timer is updated when we receive any operations(bind, query..), but
 if we do not receive any opertions in "duration", the timer expired.
 
 the operation of expire:disconnect the connection
*/
class enquire_link_timer: public smpp_timer
{
public:
	enquire_link_timer()
	{
		//default 8 seconds
		duration_ = 8 * 1000;
		timer_type_ = TIMER_ENQUIRE_LINK;
		status_ = NOT_BEGIN;
		uid_ = "enquire_link_timer";
	}
	virtual ~enquire_link_timer()
	{

	}
	virtual bool expired() 
	{
		if((status_ == END) || (status_ == NOT_BEGIN))
		{
			return false;
		}
		else
		{
			boost::posix_time::millisec_posix_time_system_config::time_duration_type duration;
			if(status_ == BEGIN)
			{
				duration = boost::posix_time::microsec_clock::local_time() - begin_time_;
			}
			else 
			{
				duration = boost::posix_time::microsec_clock::local_time() - update_time_;
			}
			boost::uint64_t past = duration.total_milliseconds();
			bool b = (past  > duration_);
			return b;
		}
	}
	virtual SMPP_TIMER_EXPIRED_ACTION process(){return DROP_SESSION;};
};
typedef boost::shared_ptr<enquire_link_timer> enquire_link_timer_ptr;
/*
the max duration betweens transactions? if any transaction produced, it will be updated,
 if there is not any transaction produced in "duration", it is expired.

there are three types of transactions:
i) messages sent from the ESME (Transmitter) to the SMSC
ii) messages sent from the SMSC to the ESME (Receiver)
iii) messages sent from the ESME (Transceiver) to the SMSC and messages sent from the SMSC to the ESME (Transceiver)

i.e. if receive any of operations below, the timer will updated
cancel_sm 
cancel_sm_resp
replace_sm 
replace_sm_resp
submit_sm
submit_sm_resp
submit_sm_multi
submit_sm_multi_resp
data_sm
data_sm_resp
deliver_sm
deliver_sm_resp
query_sm
query_sm_resp

 
 the operation of expire:disconnect the connection
*/
class inactivity_timer: public smpp_timer
{
public:
	inactivity_timer()
	{
		//default 30 seconds
		duration_ = 30 * 1000;
		timer_type_ = TIMER_INACTIVITY;
		status_ = NOT_BEGIN;
		uid_ = "inactivity_timer";
	}
	virtual ~inactivity_timer()
	{

	}

	virtual bool expired() 
	{
		if((status_ == END) || (status_ == NOT_BEGIN))
		{
			return false;
		}
		else
		{
			boost::posix_time::millisec_posix_time_system_config::time_duration_type duration;
			if(status_ == BEGIN)
			{
				duration = boost::posix_time::microsec_clock::local_time() - begin_time_;
			}
			else 
			{
				duration = boost::posix_time::microsec_clock::local_time() - update_time_;
			}
			boost::uint64_t past = duration.total_milliseconds();
			return (past  > duration_);
		}
	}
	virtual SMPP_TIMER_EXPIRED_ACTION process(){return DROP_SESSION;}
};
typedef boost::shared_ptr<enquire_link_timer> inactivity_timer_ptr;
/*
 when send a request, we should receive a response in "duration" time, 
 if do not, we should disconnect the connection

 at SMPP server side , the requests including:

deliver_sm
 
 the operation of expire:disconnect the connection
*/
class response_timer: public smpp_timer
{
public:
	response_timer()
	{
		//default 5 seconds
		duration_ = 5 * 1000;
		timer_type_ = TIMER_RESPONSE;
		status_ = NOT_BEGIN;
		uid_ = "response_timer";
	}
	virtual ~response_timer()
	{

	}
	virtual bool expired() 
	{
		if((status_ == END) || (status_ == NOT_BEGIN))
		{
			return false;
		}
		else
		{
			boost::posix_time::millisec_posix_time_system_config::time_duration_type duration;
			if(status_ == BEGIN)
			{
				duration = boost::posix_time::microsec_clock::local_time() - begin_time_;
			}
			else 
			{
				duration = boost::posix_time::microsec_clock::local_time() - update_time_;
			}
			boost::uint64_t past = duration.total_milliseconds();
			return (past  > duration_);
		}
	}
	virtual SMPP_TIMER_EXPIRED_ACTION process(){return DROP_SESSION;};
};
typedef boost::shared_ptr<response_timer> response_timer_ptr;
class smpp_timer_manager
{
public:
	smpp_timer_manager()
	{
		boost::mutex::scoped_lock lock(response_timers_mutex_);
		init_timer_duration_seconds_ = 15;
		enquire_link_timer_duration_seconds_ = 180;
		inactivity_timer_duration_seconds_ = 180;
		response_timer_duration_seconds_ = 30;
	}
	~smpp_timer_manager()
	{
		boost::mutex::scoped_lock lock(response_timers_mutex_);
		std::list< response_timer_ptr>::iterator itor_begin = response_timers_.begin();
		std::list< response_timer_ptr>::iterator itor_end = response_timers_.end();
		while(itor_begin != itor_end)
		{
			response_timers_.pop_front();
			itor_begin = response_timers_.begin();
		}
	}
protected:
	std::list<response_timer_ptr> response_timers_;
	boost::mutex response_timers_mutex_;
	session_init_timer session_init_timer_;
	enquire_link_timer enquire_link_timer_;
	inactivity_timer inactivity_timer_;

	int init_timer_duration_seconds_;
	int enquire_link_timer_duration_seconds_;
	int inactivity_timer_duration_seconds_;
	int response_timer_duration_seconds_;
	
public:
	void connected()
	{
		session_init_timer_.set_duration(init_timer_duration_seconds_ * 1000);
		session_init_timer_.begin();
		
	}
	void bind_transmitter_received()
	{
		session_init_timer_.end();
		enquire_link_timer_.set_duration(enquire_link_timer_duration_seconds_ * 1000);
		enquire_link_timer_.begin();
		inactivity_timer_.set_duration(inactivity_timer_duration_seconds_ * 1000);
		inactivity_timer_.begin();
	}
	void bind_receiver_received()
	{
		session_init_timer_.end();
		enquire_link_timer_.set_duration(enquire_link_timer_duration_seconds_ * 1000);
		enquire_link_timer_.begin();
		inactivity_timer_.set_duration(inactivity_timer_duration_seconds_ * 1000);
		inactivity_timer_.begin();
	}
	void bind_transceiver_received()
	{
		session_init_timer_.end();
		enquire_link_timer_.set_duration(enquire_link_timer_duration_seconds_ * 1000);
		enquire_link_timer_.begin();
		inactivity_timer_.set_duration(inactivity_timer_duration_seconds_ * 1000);
		inactivity_timer_.begin();
	}
	void unbind_received()
	{
		
	}
	void submit_sm_received()
	{
		inactivity_timer_.update();
		enquire_link_timer_.update();
	}
	void submit_sm_multi_received()
	{
		inactivity_timer_.update();
		enquire_link_timer_.update();
	}
	void data_sm_received()
	{
		inactivity_timer_.update();
		enquire_link_timer_.update();
	}
	void deliver_sm_resp_received()
	{
		inactivity_timer_.update();
		enquire_link_timer_.update();
	}
	void query_sm_received()
	{
		inactivity_timer_.update();
		enquire_link_timer_.update();
	}
	void cancel_sm_received()
	{
		inactivity_timer_.update();
		enquire_link_timer_.update();
	}
	void replace_sm_received()
	{
		inactivity_timer_.update();
		enquire_link_timer_.update();
	}
	void enquire_link_received()
	{
		inactivity_timer_.update(); //here may be not compatible to the smpp protocol
		enquire_link_timer_.update();
	}
	void generic_nack_received()
	{
		inactivity_timer_.update();
		enquire_link_timer_.update();
	}
	void alert_notification_received()
	{
		inactivity_timer_.update();
		enquire_link_timer_.update();
	}
	void deliver_sm_sent(Smpp::SequenceNumber seq)
	{
		boost::mutex::scoped_lock lock(response_timers_mutex_);
		std::stringstream stream;
		stream<<(Smpp::Uint32)seq;
		response_timer_ptr timer(new response_timer());
		timer->set_uid(stream.str());
		timer->set_duration(response_timer_duration_seconds_ * 1000);
		timer->begin();
		response_timers_.push_back(timer);
		inactivity_timer_.update();
		enquire_link_timer_.update();
	}
	void deliver_sm_resp_recevied(Smpp::SequenceNumber seq)
	{
		boost::mutex::scoped_lock lock(response_timers_mutex_);
		std::stringstream stream;
		stream<<(Smpp::Uint32)seq;
		std::string uid = stream.str();
		std::list< response_timer_ptr>::iterator itor = response_timers_.begin();
		for( ; itor != response_timers_.end(); )
		{
			if(itor->get()->get_uid().compare(uid) == 0 )
			{
				itor = response_timers_.erase( itor); //itor is the next element
			}
			else
			{
				itor++;
			}
		}
	}
	bool expired(SMPP_TIMER_EXPIRED_ACTION& action)
	{
		if(session_init_timer_.expired())
		{
			action = session_init_timer_.process();
			logger_log(__FUNCTION__, LEVEL_WARNING, "session_init_timer expired. ");
			return true;
		}
		if(enquire_link_timer_.expired() && (session_init_timer_.get_status() == smpp_timer::END)) //must be after init timer is end
		{
			action = enquire_link_timer_.process();
			logger_log(__FUNCTION__, LEVEL_WARNING, "enquire_link_timer expired. ");
			return true;
		}
		if(inactivity_timer_.expired() && (session_init_timer_.get_status() == smpp_timer::END))//must be after init timer is end
		{
			action = inactivity_timer_.process();
			logger_log(__FUNCTION__, LEVEL_WARNING, "inactivity_timer expired. ");
			return true;
		}
		boost::mutex::scoped_lock lock(response_timers_mutex_);
		std::list< response_timer_ptr>::iterator itor = response_timers_.begin();
		for( itor = response_timers_.begin(); itor != response_timers_.end(); )
		{
			if( itor->get()->expired()  && (session_init_timer_.get_status() == smpp_timer::END))//must be after init timer is end
			{
				action = (*itor)->process();
				logger_log(__FUNCTION__, LEVEL_WARNING, "response_timers expired, uid is %s. ", (*itor)->get_uid().c_str());
				return true;
			}
			itor++;
		}
		return false;
	}

	static void tester()
	{
		smpp_timer_manager manager;
		bool expired = false;
		SMPP_TIMER_EXPIRED_ACTION action;
		manager.connected();
		boost::this_thread::sleep(boost::posix_time::milliseconds(12000)); 
		expired = manager.expired(action);
		manager.bind_transmitter_received();
		boost::this_thread::sleep(boost::posix_time::milliseconds(12000)); 
		expired = manager.expired(action);
		manager.enquire_link_received();
		expired = manager.expired(action);
		Smpp::SequenceNumber seq = 1;
		manager.deliver_sm_sent(seq);
		manager.deliver_sm_sent(Smpp::SequenceNumber((Smpp::Uint32)2));
		manager.deliver_sm_sent(Smpp::SequenceNumber((Smpp::Uint32)1000));
		boost::this_thread::sleep(boost::posix_time::milliseconds(12000)); 
		expired = manager.expired(action);
		manager.deliver_sm_resp_recevied(Smpp::SequenceNumber((Smpp::Uint32)1));
		manager.deliver_sm_resp_recevied(Smpp::SequenceNumber((Smpp::Uint32)2));
		manager.deliver_sm_resp_recevied(Smpp::SequenceNumber((Smpp::Uint32)1000));
		expired = manager.expired(action);
		boost::this_thread::sleep(boost::posix_time::milliseconds(12000)); 
		expired = manager.expired(action); 
	}
};
#endif