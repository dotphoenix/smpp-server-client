#include "tcp_connection.hpp"
#include "protocol_data_source_manager.hpp"
protocol_data_source_manager* protocol_data_source_manager::instance_ = NULL;
boost::mutex protocol_data_source_manager::mutex_;

tcp_connection::tcp_connection(boost::asio::io_service& io_service)
	:connection(), socket_(io_service), send_buffer_size_(1024 * 1204)
{

}
tcp_connection::~tcp_connection() 
{
	close();
}

void tcp_connection::start(abstract_protocol_ptr protocol_impl)
{
	boost::mutex::scoped_lock lock(is_working_lock);
	connect_time_ = boost::posix_time::second_clock::local_time();
	remote_ip_ = socket_.remote_endpoint().address().to_string();
	remote_port_ = socket_.remote_endpoint().port();
	protocol_impl_ = protocol_impl;
	boost::asio::socket_base::send_buffer_size option1(send_buffer_size_);
	socket_.set_option(option1);
	boost::asio::socket_base::receive_buffer_size option2(send_buffer_size_);
	socket_.set_option(option2);
	logger_log(__FUNCTION__, LEVEL_INFO, "%s:%u connected to me.",remote_ip_.c_str(), remote_port_);
}

void tcp_connection::close()
{
	boost::mutex::scoped_lock lock(is_working_lock);
	if(socket_.is_open())
	{
		logger_log(__FUNCTION__, LEVEL_INFO, "gracefully close the socket %s:%u.",remote_ip_.c_str(), remote_port_);
		boost::system::error_code ignored_ec;
		socket_.close(ignored_ec);
		if(ignored_ec)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR, "close socket failed, error is %s.",boost::system::system_error(ignored_ec).what());
		}
		disconnect_time_ = boost::posix_time::second_clock::local_time();
	}
}

void tcp_connection::shutdown()
{
	boost::mutex::scoped_lock lock(is_working_lock);
	// Initiate graceful connection closure.
	if(socket_.is_open())
	{
		logger_log(__FUNCTION__, LEVEL_INFO, "gracefully shutdown the socket %s:%u.",remote_ip_.c_str(), remote_port_);
		boost::system::error_code ignored_ec;
		socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
		if(ignored_ec)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR,"shutdown socket failed, error is %s.", boost::system::system_error(ignored_ec).what());
		}
	}
	else
	{
		logger_log(__FUNCTION__, LEVEL_INFO, "we want to shutdown %s:%u, but encounter abnormal thing.",remote_ip_.c_str(), remote_port_);
	}
}

void tcp_connection::force_shutdown()
{
	std::string msg;
	if(timeout())
	{
		msg = "reason is: time out .";
	}
	else if(error_occured())
	{
		msg = "reason is: any error occurred .";
	}
	else
	{
		msg = "reason is: unknown reason .";
	}
	logger_log(__FUNCTION__, LEVEL_WARNING, "force to shutdown %s:%u, %s.", remote_ip_.c_str(), remote_port_,
			   msg.c_str());
	shutdown();

	force_remove();
}

void tcp_connection::force_reject_close()
{
	logger_log(__FUNCTION__, LEVEL_WARNING, "force to close ,it is reject .");		
	shutdown();
	close();
}
