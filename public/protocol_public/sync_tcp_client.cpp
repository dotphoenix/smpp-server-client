#include <boost/thread.hpp>
#include "algorithm.hpp"
#include "sync_tcp_client.hpp"
#include "logger.hpp"
#include <boost/asio.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>

#ifdef USING_WIN32_SOCKET
sync_tcp_client::sync_tcp_client()
{
	connected_ = false;
	socket_ = INVALID_SOCKET;
}
void sync_tcp_client::init_network()
{
	WORD wsockVersion = MAKEWORD(1,1);
	WSADATA wsaData;	
	int error =  WSAStartup(wsockVersion, &wsaData);
	if(error != 0)
	{
		error =WSAGetLastError();
		logger_log(__FUNCTION__, LEVEL_WARNING, "error is :%d ", error);
	}
}
void sync_tcp_client::uninit_network()
{
	WSACleanup();
}
int sync_tcp_client::host_ip_2_sin(const char *host_ip,unsigned short port,struct sockaddr_in *psin)
{
	struct hostent* hostent_host;
	psin->sin_family = AF_INET;
	psin->sin_port = htons(port);
	if( 0 != host_ip)
	{
		psin->sin_addr.s_addr = inet_addr(host_ip);
		if(psin->sin_addr.s_addr == INADDR_NONE)
		{
	  		hostent_host = gethostbyname(host_ip);
	  		if( 0 == hostent_host || hostent_host->h_addrtype != AF_INET)
			{
	    		memset(psin, 0, sizeof(struct sockaddr_in));
	    		return -1;
	  		}
	  		memcpy(&(psin->sin_addr), hostent_host->h_addr_list[0],sizeof(struct in_addr));
		}
	}
	else 
	{
		psin->sin_addr.s_addr = htonl(INADDR_ANY);
	}
	return 0;
}
int sync_tcp_client::socket_readable(int sockfd,int wait_mseconds)
{
	int error;
	fd_set fsReadable;
	struct timeval tv ; 
	tv.tv_sec = 0;
	tv.tv_usec = wait_mseconds * 1000;

	FD_ZERO(&fsReadable);
	FD_SET(sockfd, &fsReadable);

	error = select((int)sockfd + 1, &fsReadable, (fd_set*)NULL, (fd_set*)NULL, &tv);

	if(SOCKET_ERROR == error)
	{
		error =WSAGetLastError();
		logger_log(__FUNCTION__, LEVEL_WARNING, "error is :%d ", error);
		return -1;
	}
	if(FD_ISSET(sockfd, &fsReadable))
	{
		return 1;
	}
	return 0;
}

int sync_tcp_client::socket_writable(int sockfd, int wait_mseconds)
{
	int error;
	fd_set fsWritable;
	struct timeval tv; 
	tv.tv_sec = 0;
	tv.tv_usec = wait_mseconds * 1000;
	FD_ZERO(&fsWritable);
	FD_SET(sockfd, &fsWritable);

	error = select((int)sockfd + 1, (fd_set*)NULL, &fsWritable, (fd_set*)NULL,&tv);

	if(SOCKET_ERROR == error)
	{
		error =WSAGetLastError();
		logger_log(__FUNCTION__, LEVEL_WARNING, "error is :%d ", error);
		return -1;
	}

	if(FD_ISSET(sockfd, &fsWritable))
	{
		return 1;
	}
	return 0;
}
int sync_tcp_client::setfd_nonblock(int sockfd)
{
	unsigned long val = 1;
	return ioctlsocket(sockfd, FIONBIO, &val);
}
#ifndef min
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif
#define MTU_LENGTH 1460
#else
sync_tcp_client::sync_tcp_client():io_service_()
{
	connected_ = false;
}

#endif
sync_tcp_client::~sync_tcp_client()
{	
	disconnect(false);
}
void sync_tcp_client::disconnect(bool error_occurred)
{
	if(error_occurred)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "error occurred, disconnect the connection");
	}
	if(connected_)
	{
#ifdef USING_WIN32_SOCKET
		if(socket_ != INVALID_SOCKET)
		{
			shutdown(socket_, SD_BOTH);
			closesocket(socket_);
			socket_ = INVALID_SOCKET;
		}

#else
		boost::system::error_code ignored_ec;
		socket_ptr_->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
		if(ignored_ec)
		{
			logger_log(__FUNCTION__, LEVEL_DEBUG, "shutdown error: %s", ignored_ec.message().c_str());
		}
		socket_ptr_->close(ignored_ec);
		if(ignored_ec)
		{
			logger_log(__FUNCTION__, LEVEL_DEBUG, "close error: %s", ignored_ec.message().c_str());
		}
#endif
		connected_ = false;
		if(!error_occurred)
		{
			connection_interrupted();
		}
	}
}

void sync_tcp_client::connect(const std::string& server_ip, const std::string& server_port, const std::string& local_ip, unsigned short local_port)
{
	serverip_ = server_ip;
	serverport_ = server_port;
	localip_ = local_ip;
	localport_ = local_port;
#ifdef USING_WIN32_SOCKET
	socket_ = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr = {0};
	int ret;
	int err;
	if(localip_.length() == 0)
	{
		addr.sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		addr.sin_addr.s_addr =  inet_addr(localip_);
	}
	addr.sin_port = local_port;
	addr.sin_family = AF_INET;
	
	ret = bind(socket_, (const struct sockaddr *)&addr, sizeof(struct sockaddr_in));
	if(ret == SOCKET_ERROR)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "error is :%d ", WSAGetLastError());
		closesocket(socket_);
		socket_ = INVALID_SOCKET;
		return;
	}
	/*ret = setfd_nonblock(socket_);
	if(ret == SOCKET_ERROR)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "error is :%d ", WSAGetLastError());
		closesocket(socket_);
		socket_ = INVALID_SOCKET;
		return;
	}*/
	int buffer_size = 65535;
	if (setsockopt(socket_, SOL_SOCKET, SO_RCVBUF, (const char*)&buffer_size, sizeof(int)) == -1) 
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "set recv buffer size failed, error is :%d ", WSAGetLastError());
	}
	if (setsockopt(socket_, SOL_SOCKET, SO_SNDBUF, (const char*)&buffer_size, sizeof(int)) == -1) 
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "set send buffer size failed, error is :%d ", WSAGetLastError());
	}
	int error_count = 0;
	struct sockaddr_in sin; 
	err = host_ip_2_sin(serverip_.c_str(), datatype_convert<unsigned short>(serverport_), &sin);
  	ret = ::connect(socket_, (sockaddr*)&sin, sizeof(struct sockaddr));
  	if(ret == SOCKET_ERROR ) 
	{
		logger_log(__FUNCTION__, LEVEL_DEBUG, "connect to %s:%s failed, error is %d.",
			ip.c_str(), port.c_str(), err);
		closesocket(socket_);
		socket_ = INVALID_SOCKET;
		return;
	}
	else
	{
		logger_log(__FUNCTION__, LEVEL_DEBUG, "connect to %s:%s successfully.",	ip.c_str(), port.c_str());
		connected_ = true;
		connection_established();
		return ;
	}
#else
	try
	{
		boost::system::error_code ignored_ec;
		boost::asio::ip::tcp::endpoint local_end_point(
						boost::asio::ip::address::from_string(localip_.c_str()), localport_);
		unsigned short serverport = datatype_convert<unsigned short>(serverport_);
		boost::asio::ip::tcp::endpoint server_end_point(boost::asio::ip::address::from_string(serverip_.c_str()), serverport);
		
		/*
		 OLD IMPLEMENT, CAN NOT BIND TO LOCAL PORT SUCCESSFULLY.
		 boost::asio::ip::tcp::resolver resolver(io_service_);
		boost::asio::ip::tcp::resolver::iterator iterator;
		boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), serverip_.c_str(), serverport_.c_str(), boost::asio::ip::resolver_query_base::numeric_service);
		iterator = resolver.resolve(query);
		socket_ptr_->open(boost::asio::ip::tcp::v4(), ignored_ec);
		 boost::asio::connect(socket_, iterator, ignored_ec);
		 */

		socket_ptr_.reset(new boost::asio::ip::tcp::socket(io_service_, local_end_point));
		socket_ptr_->connect(server_end_point, ignored_ec);
		if(ignored_ec)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "connect to %s:%s(local:%s:%d) failed, error is %s.",
					   serverip_.c_str(), serverport_.c_str(), localip_.c_str(), localport_, boost::system::system_error(ignored_ec).what());
		}
		else
		{
			connected_ = true;
			connection_established();
		}
	}
	catch (std::exception& e)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "connect to %s:%s failed , error is: %s", serverip_.c_str(), serverport_.c_str(), e.what());
	}
#endif
}

bool sync_tcp_client::write(unsigned char* data, std::size_t length)
{
	if(!connected_)
	{
		return false;
	}
	if(length == 0)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "write 0 byte data is not recommended");
		return true;
	}
	std::size_t written = 0;
#ifdef USING_WIN32_SOCKET
	for(;;)
	{
		int bytes_send = 0;	
		int bytes_wantto_send = length - written;
		bytes_send = send(socket_, (const char*)data + written, bytes_wantto_send, 0);
		if(bytes_send == SOCKET_ERROR)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR, "call send() failed, errno is : %d . ", WSAGetLastError());
			return false;
		}
		written += bytes_send;
		if(written >= length)
		{
			return written == length;
		}
	}
#else
	try
	{
		written = socket_ptr_.get()->write_some(boost::asio::buffer(data, length));
		//written = boost::asio::write(*socket, boost::asio::buffer(data, length));
	}
	catch (std::exception& e)
	{
		logger_log(__FUNCTION__, LEVEL_ERROR, "write failed, reason is: %s", e.what());
		return false;
	}
#endif
	bool b = (written == length);
	if(!b)
	{
		logger_log(__FUNCTION__, LEVEL_ERROR, "write failed");
		return false;
	}
	return b;
}

bool sync_tcp_client::read(unsigned char* data, size_t length)
{
	if(!connected_)
	{
		return false;
	}
	if(length == 0)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "read 0 byte data is not recommended");
		return true;
	}
	std::size_t read = 0;
#ifdef USING_WIN32_SOCKET
	for(;;)
	{
		int bytes_wantto_recv = length - read;
		int bytes_recv = recv(socket_, (char*)data + read, bytes_wantto_recv, 0);

		if(bytes_recv == SOCKET_ERROR)
		{
			logger_log(__FUNCTION__, LEVEL_ERROR, "call recv() failed, errno is : %d .", WSAGetLastError());
			return false;
		}
		else if(bytes_recv == 0)
		{
			return false;
		}
		read += bytes_recv;
		if(read >= length)
		{
			return (read == length);
		}
	}
#else
	try
	{
		read = socket_ptr_.get()->read_some(boost::asio::buffer((void*)data, length));
	}
	catch (std::exception& e)
	{
		logger_log(__FUNCTION__, LEVEL_ERROR, "read failed, reason is: %s", e.what());
		return false;
	}
#endif
	bool b = (read == length);
	if(!b)
	{
		logger_log(__FUNCTION__, LEVEL_ERROR, "read failed");
		return false;
	}
	return b;
}

bool sync_tcp_client::read_resp(memory_buffer_ptr& bp)
{
	if(!connected_)
	{
		return false;
	}
	size_t header_length = get_header_length();
	bp.reset(new memory_buffer(header_length));
	const unsigned char* data = bp.get()->data();
	if(!read((unsigned char*)data, header_length))
	{
		return false;
	}
	size_t total_length = get_total_length_from_header((unsigned char*)data, header_length);
	if(total_length > get_max_body_length() + get_header_length())
	{
		logger_log(__FUNCTION__, LEVEL_ERROR, "total_length %u is too large", (unsigned int)total_length);
		disconnect(true);
		return false;
	}
	if(total_length > header_length)  
	{
		bp.get()->resize(total_length); 
		const unsigned char* data = bp.get()->data();
		if(!read((unsigned char*)data + header_length, total_length - header_length))
		{
			return false;
		}
		return true;
	}
	else
	{
		return true;
	}
}

bool sync_tcp_client::connect_for(const std::string& server_ip, const std::string& server_port, int milliseconds,
								  const std::string& local_ip, unsigned short local_port)
{
	#define WAIT_TIME 1000
	int connect_times = (int)(milliseconds / WAIT_TIME);
	while((connect_times--) > 0)
	{
		connect(server_ip, server_port, local_ip, local_port);
		if(is_connected())
		{
			return true;
		}
		boost::this_thread::sleep(boost::posix_time::milliseconds(WAIT_TIME));
	}
	return false;
}

std::string sync_tcp_client::get_local_default_ip_v4()
{
	try
	{
		boost::asio::io_service io_service;
		  
		boost::asio::ip::tcp::resolver resolver(io_service);
		boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(),"");
		boost::asio::ip::tcp::resolver::iterator it=resolver.resolve(query);
		  
		while(it != boost::asio::ip::tcp::resolver::iterator())
		{
			boost::asio::ip::address addr = (it++)->endpoint().address();
			if(addr.is_v6())
			{
				continue;
			}
			else
			{
				if(!addr.is_loopback())
				{
					return addr.to_string();
				}
			}
		}
	}
	catch(std::exception &e)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "failed, reason is %s", e.what());
	}
	logger_log(__FUNCTION__, LEVEL_WARNING, "failed.");
	return "";
}

