#ifndef CONNECTIONS_MANAGER_HPP
#define CONNECTIONS_MANAGER_HPP
#include <set>
#include <queue>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/optional.hpp>
#include "connection.hpp"
#include "logger.hpp"
#include "memory_buffer.hpp"
class connections_manager
{
public:
	connections_manager( );
	~connections_manager();

	void add(connection_ptr client);
	void remove(connection_ptr client);
	boost::optional<connection_ptr> get(); /*means get a connection to process or send data*/
	void free(connection_ptr s); /*the processing is finished*/
	size_t current_size();

private:	
	std::set<connection_ptr> connections_;
	boost::mutex mutex_;
	bool finished_; //if finished , we do not accept any connect request or add buffer request
public:
	void finish(){finished_ = true;}
	void stop_all();
};
#endif