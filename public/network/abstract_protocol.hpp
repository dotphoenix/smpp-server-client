/*
 *  abstract_protocol.h
 *
 *  Created by Zhu Nengjie on 7/25/13.
 *  Copyright 2013 Bifang technology. All rights reserved.
 *
 */
#ifndef ABSTRACT_PROTOCOL_H
#define ABSTRACT_PROTOCOL_H
#include "memory_buffer.hpp"
#include "logger.hpp"
#include <boost/thread/mutex.hpp>
#include <queue>
#include "protocol_data_source_manager.hpp"
class abstract_protocol
{
public:
	abstract_protocol(){error_occured_ = false;}
	virtual ~abstract_protocol(){}
protected:
	std::string tcp_client_ip_;
	unsigned short tcp_client_port_;
	std::string data_source_flag_;
	bool error_occured_;
	
	/*make sure the operations which arrive earlier be processed completed earlier,
	 because, there may be more then one threads to process the operations in a 
	 same smpp_impl object
	 */
	boost::mutex process_operation_mutex_;
public:
	protocol_data_source private_data_source_;
	virtual void process(memory_buffer_ptr& bp) = 0;
	virtual bool get_from_out_queue(memory_buffer_ptr& bp)
	{
		return false;
	}
	virtual bool get_from_in_queue(memory_buffer_ptr& bp)
	{
		bool has = private_data_source_.get_from_in_queue(bp);
		if(data_source_flag_.length() == 0)
		{
			return has;
		}
		if(!has)
		{
			has = protocol_data_source_manager::get_instance()->get_from_in_queue(data_source_flag_, bp);
		}
		return has;
	}
	virtual void add_to_in_queue(memory_buffer_ptr bp)
	{
		if(data_source_flag_.length() > 0)
		{
			protocol_data_source_manager::get_instance()->add_to_in_queue(data_source_flag_, bp);
		}
		else
		{
			private_data_source_.add_to_in_queue(bp);
		}
	}
	virtual bool any_error_occured()
	{
		boost::mutex::scoped_lock lock(process_operation_mutex_);
		return error_occured_;
	}

	virtual bool need_send(){return false;}
	virtual bool need_process(){return false;}
	virtual bool timer_timeout(){return false;};
	
	virtual void tcp_connection_ip(const std::string& ip, unsigned short port)
	{tcp_client_ip_ = ip; tcp_client_port_ = port;}
	
	virtual size_t get_header_length() = 0;
	virtual size_t get_total_length_from_header(memory_buffer_ptr& bp) = 0;
	virtual void set_data_source_flag(const std::string& flag){data_source_flag_ = flag;}
	const std::string& get_data_source_flag(){return data_source_flag_;}
}
;
typedef boost::shared_ptr<abstract_protocol> abstract_protocol_ptr;
#endif
