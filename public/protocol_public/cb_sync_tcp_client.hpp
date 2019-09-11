#ifndef CB_SYNC_TCP_CLIENT_H
#define CB_SYNC_TCP_CLIENT_H
#include "memory_buffer.hpp"
#include "sync_tcp_client.hpp"
#include "cb_protocol_structure.hpp"
#include "logger.hpp"
class cb_sync_tcp_client: public sync_tcp_client
{
protected:
	bool parse_header(unsigned char* data, size_t length, cb_header& header)
	{
		header.unpack_header((const unsigned char*)data);

		return true;
	}
	virtual size_t get_header_length(){return cb_header::Length;}
	virtual size_t get_total_length_from_header(unsigned char* data, size_t length)
	{
		cb_header header;
		if(parse_header(data, length, header))
		{
			return header.command_length;
		}
		return get_header_length();
	}
	
	virtual void connection_established(){}
	virtual void connection_interrupted(){}
};

#endif