#include <boost/format.hpp>
#include <sstream>
#include <assert.h>
#include "connections_manager.hpp"
#include "tcp_connection_impl.hpp"

connections_manager::connections_manager( )
{
	finished_ = false;
}
connections_manager::~connections_manager()
{

}
size_t connections_manager::current_size()
{
	boost::mutex::scoped_lock lock(mutex_);
	size_t size = connections_.size();
	return size;
}
void connections_manager::add(connection_ptr client)
{
	boost::mutex::scoped_lock lock(mutex_);
	if(!finished_)
	{
		connections_.insert(client);
	}
}

void connections_manager::remove(connection_ptr client)
{
	boost::mutex::scoped_lock lock(mutex_);
	bool is_in = (connections_.find(client) != connections_.end());
	if(is_in)
	{
		client->close();
		connections_.erase(client);
	}
	else
	{
		logger_log(__FUNCTION__, LEVEL_WARNING,"remove a no exist connection.");
	}
}
/*means get a connection to process or send data*/
boost::optional<connection_ptr> connections_manager::get()
{
	boost::mutex::scoped_lock lock(mutex_);
	boost::optional<connection_ptr> s(boost::none);
	if(!connections_.empty())
	{
		std::set<connection_ptr>::iterator it;
		for (it = connections_.begin(); it != connections_.end(); ++it)    
		{
			if(!(*it)->is_working())
			{
				if((*it)->need_process() || (*it)->need_send() || (*it)->error_occured() || (*it)->timeout())
				{
					s = *it;
					(*it)->set_working(true);
					return s;
				}
			}
		}
	}
	return s;
}
void connections_manager::free(connection_ptr s)/*the processing is finished*/
{
	boost::mutex::scoped_lock lock(mutex_);
	s->set_working(false);
}


void connections_manager::stop_all()
{
	finished_ = true;
	std::for_each(connections_.begin(), connections_.end(),
				  boost::bind(&connection::force_shutdown, _1));
	connections_.clear();
}