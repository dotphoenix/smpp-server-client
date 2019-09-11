#ifndef TCP_CONNECTION_IMPL_HPP
#define TCP_CONNECTION_IMPL_HPP
#include <queue>
#include <boost/thread/mutex.hpp>
#include "tcp_connection.hpp"
#include "abstract_protocol.hpp"

/*
architecture:
 each tcp_connection_impl represents a real tcp session, the connection(the server.cpp) drives the process.

the implemention of protocol_session interface process the detailed of logics
*/
class tcp_connection_impl:public tcp_connection,
public boost::enable_shared_from_this<tcp_connection_impl>
{
public:
	tcp_connection_impl(boost::asio::io_service& io_service, connections_manager& cm);
	~tcp_connection_impl();

public:
	virtual void start(abstract_protocol_ptr protocol_impl);
	virtual bool timeout(){return protocol_impl_->timer_timeout() || read_timeout_;}
	virtual bool error_occured(){return protocol_impl_->any_error_occured();}
	virtual void force_remove();
protected:
	void write(void* data, std::size_t length);
	void read_header();
	void read_body(memory_buffer_ptr bp);
	void handle_read_header(const boost::system::error_code& error, memory_buffer_ptr bp);
	void handle_read_body(const boost::system::error_code& error, memory_buffer_ptr bp);
	void handle_write(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_timeout(const boost::system::error_code& e);

protected:
	connections_manager& cm_;	
	bool read_timeout_;
	boost::asio::deadline_timer read_timer_;
	int read_timer_duration;
public:
	virtual void do_send();
	virtual bool need_send();
	virtual bool need_process();
	virtual void do_process();

};

typedef boost::shared_ptr<tcp_connection_impl> tcp_connection_impl_ptr;

#endif