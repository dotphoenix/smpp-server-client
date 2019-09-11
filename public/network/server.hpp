#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <boost/interprocess/shared_memory_object.hpp>  
#include <boost/interprocess/mapped_region.hpp>  
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/asio.hpp>
#include <boost/thread/condition.hpp>
#include <string>
#include <boost/optional.hpp>
#include <assert.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "connections_manager.hpp"
#include "tcp_connection_impl.hpp"

struct listen_address
{
	std::string ip;
	std::string port;
};

class server
{
public:
	server(const std::vector<listen_address>& listen_addrs, std::size_t task_thread_nb);
	
	void handle_accept(const boost::system::error_code& error, size_t index);
	void start_accept();
	void run();
	void stop();
	void finish() {finished_ = true;}
private:
	//set finish, we do not accept any connections , but we should still process the data in the buffer
	bool finished_;
	boost::asio::io_service io_service_;	
	boost::shared_ptr<boost::thread> io_thread_;
	connections_manager cm_;
	
	std::vector<boost::shared_ptr<tcp::acceptor> > acceptors_;
	std::vector<listen_address> listen_addrs_;
	
	void task_runner();
	std::size_t task_thread_nb_;
	std::vector< boost::shared_ptr<boost::thread> > task_threads_;
	boost::condition task_coming_;

	tcp_connection_impl_ptr new_connection_;
};
typedef boost::shared_ptr<server> server_ptr;
#endif