#include "server.hpp"
#ifdef CB_SERVER
#include "cb_protocol_impl.hpp"
#elif defined CB_SMPPSERVER
#include "smpp_protocol_impl.hpp"
#else
#pragma message  ("WARNING!!!WARNING!!! Using  " __FILE__ "  must implement abstract_protocol interface ")
#endif

#define MAX_SUPPORT_CLIENTS 999
server::server(const std::vector<listen_address>& listen_addrs,  
			   std::size_t task_thread_nb)
:finished_(false), cm_(),
task_thread_nb_((task_thread_nb > 4) ? task_thread_nb : 4),
new_connection_()
{
	
	boost::asio::ip::tcp::resolver resolver(io_service_);
	
	listen_addrs_ = listen_addrs;
	for(size_t s = 0; s < listen_addrs_.size(); s++)
	{
		listen_address& la = listen_addrs_.at(s);
		boost::shared_ptr<tcp::acceptor> acptr(new tcp::acceptor(io_service_));
		boost::asio::ip::tcp::resolver::query query(la.ip, la.port);
		boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
		acptr->open(endpoint.protocol());
		acptr->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acptr->bind(endpoint);
		acptr->listen();
		acceptors_.push_back(acptr);
	}
	start_accept();
}


void server::start_accept()
{
	new_connection_.reset(new tcp_connection_impl(io_service_, cm_));
	for(size_t s = 0; s < acceptors_.size(); s++)
	{
		acceptors_.at(s)->async_accept(new_connection_->socket(),
		boost::bind(&server::handle_accept, this,
		boost::asio::placeholders::error, s));
	}
	
}

void server::handle_accept(const boost::system::error_code& error, size_t index)
{
	// Check whether the server was stopped by a signal before this completion
	// handler had a chance to run.
	if (!acceptors_.at(index)->is_open())
	{
		logger_log(__FUNCTION__, LEVEL_ERROR, "failed, acceptor is not open.");		
		return;
	}
	if (!error)
	{
		if(cm_.current_size() > MAX_SUPPORT_CLIENTS)
		{
			new_connection_->force_reject_close();
		}
		else
		{
#ifdef CB_SERVER //should use polymorphism, not macro, it is not gracefull
			abstract_protocol_ptr protocol_impl(new cb_protocol_impl());
			new_connection_->start(protocol_impl);
#elif defined CB_SMPPSERVER
			abstract_protocol_ptr protocol_impl(new smpp_impl());
			new_connection_->start(protocol_impl);			
#endif
			
		}
		if(!finished_)
		{
			start_accept();
		}
	}
	else
	{
		logger_log(__FUNCTION__, LEVEL_ERROR, "failed reason is %s.", boost::system::system_error(error).what());		
		if(!finished_)
		{
			start_accept();
		}
	}
}

void server::task_runner()
{
	while(!finished_)
	{
		boost::optional<connection_ptr> s = cm_.get();
		if(s)
		{
			connection_ptr cp = s.get();
			tcp_connection_impl* c = (tcp_connection_impl*)cp.get();
			if(c->need_send())
			{
				c->do_send();
			}
			if(c->need_process())
			{
				c->do_process();
			}
			if(c->need_send())
			{
				c->do_send();
			}
			if(c->error_occured() || c->timeout())
			{
				c->force_shutdown();
			}
			cm_.free(cp);
		}
		else
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(50));
		}
	}
}

void server::run()
{
	std::size_t s = 0;
	try
	{
		io_thread_.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
	}
	catch (std::exception& ex)
	{
		std::string what = ex.what();
		logger_log(__FUNCTION__, LEVEL_ERROR, "error: %s ." , what.c_str());
	}

	for(s = 0; s < task_thread_nb_; s++) 
	{
	
		boost::shared_ptr<boost::thread> thread(
										new boost::thread(boost::bind(&server::task_runner, this)));
		task_threads_.push_back(thread);
	}
	
	while(!finished_)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	}
}

void server::stop()
{
	finished_ = true;
	std::size_t s = 0;
	for(s = 0; s < acceptors_.size(); s++)
	{
		acceptors_.at(s)->close();
	}
	
	cm_.stop_all();
	io_service_.stop();
	
	// Wait for all threads in the pool to exit.
	for(s = 0; s < task_threads_.size(); ++s)
	{
		task_threads_[s]->join();
	}
	io_thread_->join();
}
