#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP
#include <string>
#include <boost/cstdint.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "logger.hpp"
#include "connections_manager.hpp"
#include "connection.hpp"
using boost::asio::ip::tcp;
class tcp_connection :public connection
{
public:
	tcp_connection(boost::asio::io_service& io_service);
	virtual ~tcp_connection() ;
public:
	virtual void start(abstract_protocol_ptr protocol_impl);
	virtual void close();
	virtual void force_shutdown() ;
	virtual void force_reject_close();

public:
	virtual tcp::socket& socket(){return socket_;}
	virtual void write(void* data, std::size_t length) = 0;

protected:
	std::string remote_ip_;
	unsigned short remote_port_;
	tcp::socket socket_;
	void shutdown();
	int send_buffer_size_;
};
typedef boost::shared_ptr<tcp_connection> tcp_connection_ptr;
#endif
