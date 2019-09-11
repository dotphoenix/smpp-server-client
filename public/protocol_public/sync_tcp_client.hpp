#ifndef SYNC_TCP_CLIENT_H
#define SYNC_TCP_CLIENT_H
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include "memory_buffer.hpp"
#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
//#define USING_WIN32_SOCKET
#endif
#include <boost/asio.hpp>

// this class is not thread safety
class sync_tcp_client
{
public:
	sync_tcp_client();
	~sync_tcp_client();
#ifdef USING_WIN32_SOCKET
	static void init_network();
	static void uninit_network();
	static int host_ip_2_sin(const char *host_ip,unsigned short port,struct sockaddr_in *psin);
	static int socket_readable(int sockfd, int wait_mseconds);
	static int socket_writable(int sockfd, int wait_mseconds);
	static int setfd_nonblock(int sockfd);
#endif
	//if don't want to bind local ip and port, pass local_ip to "", pass local_port to 0
	void connect(const std::string& server_ip, const std::string& server_port,
				 const std::string& local_ip, unsigned short local_port);
	void disconnect(bool error_occurred);
	bool is_connected(){return connected_;}
	bool write(unsigned char* data, size_t length);
	bool read(unsigned char* data, size_t length);
public:
	virtual void connection_established(){}
	virtual void connection_interrupted(){}
protected:
	std::string serverip_;
	std::string serverport_;
	std::string localip_;
	unsigned short localport_;
	boost::asio::ip::tcp::endpoint local_end_point;
#ifndef USING_WIN32_SOCKET
	boost::asio::io_service io_service_;
	boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr_;
#else
	SOCKET socket_;
#endif
	bool connected_;


protected:
	virtual size_t get_header_length() = 0;
	virtual size_t get_max_body_length() = 0;
	virtual size_t get_total_length_from_header(unsigned char* data, size_t length) = 0;
public:
	
	bool read_resp(memory_buffer_ptr& bp);
	void bind_local(const std::string& ip, unsigned short port);
	//try to connect for milliseconds until connected
	bool connect_for(const std::string& server_ip, const std::string& server_port, int milliseconds,
					 const std::string& local_ip, unsigned short local_port);
	
	public:
	static std::string get_local_default_ip_v4();
};
typedef boost::shared_ptr<sync_tcp_client> sync_tcp_client_ptr;
#endif
