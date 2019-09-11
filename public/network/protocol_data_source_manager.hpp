#ifndef __protocol_data_source_manager__
#define __protocol_data_source_manager__
#include "algorithm.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <boost/optional.hpp>
#include <time.h>
#include "logger.hpp"

class protocol_data_source
{
public:
	protocol_data_source()
	{
		last_update_time_ = time(NULL);
	}
protected:
	std::queue<memory_buffer_ptr> in_queue_;
	boost::mutex in_queue_mutex_;
	
	std::queue<memory_buffer_ptr> out_queue_;
	boost::mutex out_queue_mutex_;
	
	time_t last_update_time_;
public:
	void add_to_in_queue(memory_buffer_ptr bp)
	{
		boost::mutex::scoped_lock lock(in_queue_mutex_);
		in_queue_.push(bp);
		last_update_time_ = time(NULL);
	}
	
	bool get_from_in_queue(memory_buffer_ptr& bp)
	{
		boost::mutex::scoped_lock lock(in_queue_mutex_);
		if(in_queue_.size() > 0)
		{
			bp = in_queue_.front();
			in_queue_.pop();
			return true;
		}
		return false;
	}
	void add_to_out_queue(memory_buffer_ptr bp)
	{
		boost::mutex::scoped_lock lock(out_queue_mutex_);
		out_queue_.push(bp);
		last_update_time_ = time(NULL);
	}
	
	bool get_from_out_queue(memory_buffer_ptr& bp)
	{
		boost::mutex::scoped_lock lock(out_queue_mutex_);
		if(out_queue_.size() > 0)
		{
			bp = out_queue_.front();
			out_queue_.pop();
			return true;
		}
		return false;
	}

	size_t in_queue_size()
	{
		boost::mutex::scoped_lock lock(in_queue_mutex_);
		return in_queue_.size();
	}
	size_t out_queue_size()
	{
		boost::mutex::scoped_lock lock(out_queue_mutex_);
		return out_queue_.size();
	}
	bool need_process(){return in_queue_size() > 0;}
	bool need_send(){return out_queue_size() > 0;}
	
	bool timeout()
	{
		time_t now = time(NULL);
		if(now - last_update_time_ > 120)
		{
			return true;
		}
		return false;
	}
	void clear_packets()
	{
		{
			boost::mutex::scoped_lock lock(out_queue_mutex_);
			if(!out_queue_.empty())
			{
				size_t n = out_queue_.size();
				while(!out_queue_.empty())
				{
					out_queue_.pop();
				}
				logger_log(__FUNCTION__, LEVEL_WARNING, "discard %u packets in out queue", (unsigned int)n);
			}
			
		}
		{
			boost::mutex::scoped_lock lock(in_queue_mutex_);
			if(!in_queue_.empty())
			{
				size_t n = in_queue_.size();
				while(!in_queue_.empty())
				{
					in_queue_.pop();
				}
				logger_log(__FUNCTION__, LEVEL_WARNING, "discard %u packets in in queue", (unsigned int)n);
			}
			
		}
	}
};

class connection_data_source
{
	
public:
	connection_data_source(const std::string& flag){flag_ = flag;}
	protocol_data_source A_; //in smpp , out queue of A saves submit resp
	protocol_data_source B_; //in smpp , out queue of B saves dlr & mo
	std::string flag_;
	void clear_packets()
	{
		A_.clear_packets();
		B_.clear_packets();
	}
};
typedef boost::shared_ptr<connection_data_source> connection_data_source_ptr;

class protocol_data_source_manager
{
public:
	static protocol_data_source_manager* get_instance()
	{
		boost::mutex::scoped_lock lock(mutex_);
		if(instance_ == NULL)
		{
			instance_ = new protocol_data_source_manager();
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
	protocol_data_source_manager(){}
	virtual ~protocol_data_source_manager(){}
	static protocol_data_source_manager* instance_;
	static boost::mutex mutex_;
	
protected:
	std::list<connection_data_source_ptr> cdsps_;
	boost::mutex cdsps_mutex_;
	boost::optional<connection_data_source_ptr> get_cdsp_no_lock(const std::string& flag)
	{
		boost::optional<connection_data_source_ptr> o(boost::none);
		if(!cdsps_.empty())
		{
			std::list<connection_data_source_ptr>::iterator it;
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
	void add_to_in_queue(const std::string& flag, memory_buffer_ptr bp)
	{
		if(flag.length() == 0)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "flag is empty.");
			return ;
		}
		boost::mutex::scoped_lock lock(cdsps_mutex_);
		boost::optional<connection_data_source_ptr> o = get_cdsp_no_lock(flag);
		if(o)
		{
			return o.get()->A_.add_to_in_queue(bp);
		}
		else
		{
			connection_data_source_ptr cdsp(new connection_data_source(flag));
			cdsp->A_.add_to_in_queue(bp);
			cdsps_.push_back(cdsp);
		}
	}
	void add_to_out_queue_A(const std::string& flag, memory_buffer_ptr bp)
	{
		if(flag.length() == 0)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "flag is empty.");
			return ;
		}
		
		boost::mutex::scoped_lock lock(cdsps_mutex_);
		boost::optional<connection_data_source_ptr> o = get_cdsp_no_lock(flag);
		if(o)
		{
			return o.get()->A_.add_to_out_queue(bp);
		}
		else
		{
			connection_data_source_ptr cdsp(new connection_data_source(flag));
			cdsp->A_.add_to_out_queue(bp);
			cdsps_.push_back(cdsp);
		}
	}
	bool get_from_in_queue(const std::string& flag, memory_buffer_ptr& bp)
	{
		boost::mutex::scoped_lock lock(cdsps_mutex_);
		boost::optional<connection_data_source_ptr> o = get_cdsp_no_lock(flag);
		if(o)
		{
			return o.get()->A_.get_from_in_queue(bp);
		}
		return false;
	}
	bool get_from_out_queue_A(const std::string& flag, memory_buffer_ptr& bp)
	{
		boost::mutex::scoped_lock lock(cdsps_mutex_);
		boost::optional<connection_data_source_ptr> o = get_cdsp_no_lock(flag);
		if(o)
		{
			return o.get()->A_.get_from_out_queue(bp);
		}
		return false;
	}

	void add_to_out_queue_B(const std::string& flag, memory_buffer_ptr bp)
	{
		if(flag.length() == 0)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "flag is empty.");
			return ;
		}
		boost::mutex::scoped_lock lock(cdsps_mutex_);
		boost::optional<connection_data_source_ptr> o = get_cdsp_no_lock(flag);
		if(o)
		{
			return o.get()->B_.add_to_out_queue(bp);
		}
		else
		{
			connection_data_source_ptr cdsp(new connection_data_source(flag));
			cdsp->B_.add_to_out_queue(bp);
			cdsps_.push_back(cdsp);
		}
	}
	bool get_from_out_queue_B(const std::string& flag, memory_buffer_ptr& bp)
	{
		boost::mutex::scoped_lock lock(cdsps_mutex_);
		boost::optional<connection_data_source_ptr> o = get_cdsp_no_lock(flag);
		if(o)
		{
			return o.get()->B_.get_from_out_queue(bp);
		}
		return false;
	}
	bool need_process(const std::string& flag)
	{
		boost::mutex::scoped_lock lock(cdsps_mutex_);
		boost::optional<connection_data_source_ptr> o = get_cdsp_no_lock(flag);
		if(o)
		{
			return (o.get()->A_.need_process() || o.get()->B_.need_process());
		}
		return false;
	}
	bool need_send_A(const std::string& flag)
	{
		boost::mutex::scoped_lock lock(cdsps_mutex_);
		boost::optional<connection_data_source_ptr> o = get_cdsp_no_lock(flag);
		if(o)
		{
			return o.get()->A_.need_send();
		}
		return false;
	}
	bool need_send_B(const std::string& flag)
	{
		boost::mutex::scoped_lock lock(cdsps_mutex_);
		boost::optional<connection_data_source_ptr> o = get_cdsp_no_lock(flag);
		if(o)
		{
			return o.get()->B_.need_send();
		}
		return false;
	}
	void clear_packets(const std::string& flag)
	{
		boost::mutex::scoped_lock lock(cdsps_mutex_);
		boost::optional<connection_data_source_ptr> o = get_cdsp_no_lock(flag);
		if(o)
		{
			o.get()->clear_packets();
		}
	}
};
#endif /* defined(__protocol_data_source_manager__) */
