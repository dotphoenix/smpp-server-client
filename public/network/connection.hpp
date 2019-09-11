#ifndef CONNECTION_HPP
#define CONNECTION_HPP
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "abstract_protocol.hpp"
#define MTU 1280
class connection
{
public:
	connection()
	{
		is_working_ = false;
	}
	virtual ~connection() 
	{
	}
	virtual void start(abstract_protocol_ptr protocol_impl) = 0;
	virtual void close() = 0;
	
	//if timeout or error occurs, we shutdown the socket
	virtual void force_shutdown() = 0 ;
	
	//shutdown and close the socket, usually we reject to accept this socket
	virtual void force_reject_close() = 0;
	
	bool is_working()
	{
		boost::mutex::scoped_lock lock(is_working_lock);
		return is_working_;
	}
	void set_working(bool working)
	{
		boost::mutex::scoped_lock lock(is_working_lock);
		is_working_ = working;
	}

protected:
	bool is_working_;
	boost::mutex is_working_lock; 
	boost::posix_time::ptime connect_time_;
	boost::posix_time::ptime disconnect_time_;
protected:
	abstract_protocol_ptr protocol_impl_;
public:
	virtual bool need_process(){return false;}
	virtual bool need_send(){return false;}
	virtual bool timeout(){return false;}
	virtual bool error_occured(){return false;}
	virtual void force_remove(){}
};
typedef boost::shared_ptr<connection> connection_ptr;
#endif
