#include <boost/format.hpp>
#include "tcp_connection_impl.hpp"

tcp_connection_impl::tcp_connection_impl(boost::asio::io_service& io_service, connections_manager& cm)
:tcp_connection(io_service), cm_(cm), read_timeout_(false), read_timer_(io_service),read_timer_duration(720)
{

}
tcp_connection_impl::~tcp_connection_impl()
{
	read_timer_.cancel();
}

void tcp_connection_impl::start(abstract_protocol_ptr protocol_impl )
{
	tcp_connection::start(protocol_impl);
	cm_.add(shared_from_this());
	protocol_impl_->tcp_connection_ip(remote_ip_, remote_port_);
	read_header();
	read_timer_.async_wait(boost::bind(&tcp_connection_impl::handle_timeout,this,
									   boost::asio::placeholders::error));
}

void tcp_connection_impl::read_header()
{
	size_t header_length = protocol_impl_->get_header_length();
	memory_buffer_ptr bp(new memory_buffer(header_length));
	boost::asio::async_read(socket_,
		boost::asio::buffer((void*)(bp.get()->data()), header_length),
		boost::bind(
		&tcp_connection_impl::handle_read_header, shared_from_this(),
		boost::asio::placeholders::error,  bp));
	read_timer_.expires_from_now(boost::posix_time::seconds(read_timer_duration));

}
void tcp_connection_impl::read_body(memory_buffer_ptr bp)
{
	size_t header_length = protocol_impl_->get_header_length();
	size_t total_length = protocol_impl_->get_total_length_from_header(bp);
	
	boost::asio::async_read(socket_,
		boost::asio::buffer((void*)(bp.get()->data() + header_length), 
		total_length - header_length),
		boost::bind(&tcp_connection_impl::handle_read_body, shared_from_this(),
		boost::asio::placeholders::error, bp));
}
void tcp_connection_impl::write(void* data, std::size_t length)
{
	if(length == 0)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "write 0 byte data is not recommended");
		return ;
	}
	boost::asio::async_write(socket_,
		boost::asio::buffer(data, length),
		boost::asio::transfer_at_least(length),
		boost::bind(&tcp_connection_impl::handle_write, shared_from_this(),
		boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}
void tcp_connection_impl::handle_timeout(const boost::system::error_code& e)
{
	if (e != boost::asio::error::operation_aborted)
	{
		read_timeout_ = true;
		logger_log(__FUNCTION__, LEVEL_ERROR, "read timeout...");
        /*call the functions under may call exceptional */
		//cm_.remove(shared_from_this());
		//force_shutdown();
	}
	else
	{
	}            
	return;
}
void tcp_connection_impl::handle_read_header(const boost::system::error_code& error, memory_buffer_ptr bp)
{
	if (!error)
	{
		size_t header_length = protocol_impl_->get_header_length();
		size_t total_length = protocol_impl_->get_total_length_from_header(bp);
		if(total_length > header_length)
		{
			bp.get()->resize(total_length); // resize to the number of octets.
			read_body(bp);
		}
		else
		{
			//some commands only including header, no body, such as enquire_link
			protocol_impl_->add_to_in_queue(bp);
			read_header();					
		}
	}
	else
	{
		logger_log(__FUNCTION__, LEVEL_INFO, "%s:%u disconnected from me, reason is %s.",
			remote_ip_.c_str(), remote_port_, boost::system::system_error(error).what());		
		if (error != boost::asio::error::operation_aborted)
		{
			cm_.remove(shared_from_this());
		}
		else
		{
		}
	}
}

void tcp_connection_impl::handle_read_body(const boost::system::error_code& error, memory_buffer_ptr bp)
{
	if (!error)
	{
		protocol_impl_->add_to_in_queue(bp);
		//read another header
		read_header();
	}
	else
	{
		logger_log(__FUNCTION__, LEVEL_INFO, "%s:%u disconnected from me, reason is %s.",
			remote_ip_.c_str(), remote_port_, boost::system::system_error(error).what());		

		if (error != boost::asio::error::operation_aborted)
		{
			cm_.remove(shared_from_this());
		}
		else
		{

		}
	}
}

void tcp_connection_impl::handle_write(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (!error)
	{
	}
	else
	{
		logger_log(__FUNCTION__, LEVEL_INFO, "%s:%u disconnected from me, reason is %s.",
			remote_ip_.c_str(), remote_port_, boost::system::system_error(error).what());		
		if (error != boost::asio::error::operation_aborted)
		{
			cm_.remove(shared_from_this());
		}
	}
}

void tcp_connection_impl::do_send()
{
	while(true)
	{
		memory_buffer_ptr bp;
		bool has = protocol_impl_->get_from_out_queue(bp);
		if(!has)
		{
			break;
		}
		const unsigned char* data = bp.get()->data();
		size_t total_length = bp.get()->size();
		size_t remain = total_length;
		size_t tmp_sent = 0;
		while(remain > 0)
		{
			size_t want_write = std::min(remain, (size_t)MTU);
			write((void*)data, want_write);
			data += want_write;
			remain -= want_write;
			tmp_sent += want_write;
			if(tmp_sent >= (size_t)send_buffer_size_)
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(20)); 
				tmp_sent = 0;
			}
			
		}
	}
}
void tcp_connection_impl::do_process()
{
	while(true)
	{
		memory_buffer_ptr bp;
		bool has = protocol_impl_->get_from_in_queue(bp);
		if(has)
		{
			protocol_impl_->process(bp);
			do_send();
		}
		else 
		{
			break;
		}

	}
	
}

bool tcp_connection_impl::need_process()
{
	return protocol_impl_->need_process();
}
bool tcp_connection_impl::need_send()
{
	return protocol_impl_->need_send();
}

void tcp_connection_impl::force_remove()
{
	cm_.remove(shared_from_this());
}