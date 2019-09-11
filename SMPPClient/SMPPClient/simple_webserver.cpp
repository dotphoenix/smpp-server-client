#include "algorithm.hpp"
#include <stdio.h>
#include <string.h>
#include <vector>
#include "base64.h"
#ifndef _WIN32
#include <unistd.h>
#else
#endif
#include "mongoose.h"
#include <boost/shared_ptr.hpp>
#include "cb_protocol_public.hpp"
#include "cb_client_api.h"
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#define MAX_THREADS 4
static struct mg_server *server[MAX_THREADS];
static std::string GET_METHOD = "GET";
static std::string POST_METHOD = "POST";
static std::string HEAD_METHOD = "HEAD";

static bool stop_webserver = false;
static std::string s_port = "8000";
static std::string s_root_fullpath = "./";
const char* response_headers =
		  "HTTP/1.1 200 OK\r\n"
		  "Content-Length:%d\r\n"
		  "Content-Type: text/html\r\n\r\n";
const char* response_content =
		  "<html>\r\n"
		  "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF8\" />\r\n"
		  "<body>\r\n"
		  "%s\r\n"
		  "</body>\r\n"
		  "</html>\r\n";
static std::string get_submit = "/get_submit";
static std::string get_mos = "/get_mos";
static std::string get_dlrs = "/get_dlrs";
static std::string get_statistic = "/get_statistic";
static std::string get_balance = "/get_balance";

static std::string html_br = "<br>";
static std::string html_blank = "&nbsp;";


static void response_200_full_html(struct mg_connection *conn, const std::string& body)
{
	int len = (int)(body.length() + 100 + strlen(response_headers) + strlen(response_content));
	memory_buffer_ptr bp1, bp2;
	bp1.reset(new memory_buffer(len));
	bp2.reset(new memory_buffer(len));
	sprintf((char*)(bp1->data()), response_content, body.c_str());
	sprintf((char*)(bp2->data()), response_headers, (int)strlen((char*)(bp1->data())));
	//std::string full_html = std::string((char*)(bp2->data())) + std::string((char*)(bp1->data()));
	std::string full_html = std::string((char*)(bp1->data()));
	mg_send_data(conn, (void*)full_html.c_str(), (int)full_html.length());
}

static void response_200_plain_text(struct mg_connection *conn, const std::string& text)
{
	mg_send_data(conn, (void*)text.c_str(), (int)text.length());
}

void process_get_balance(struct mg_connection *conn)
{
	response_200_full_html(conn, datatype_convert<std::string>(100000));
}


static void response_invalid_parameter(struct mg_connection *conn)
{
	response_200_full_html(conn, "400");
}

static int process_get_submit(struct mg_connection* conn)
{
#define LENGTH_1 4096
#define LENGTH_2 (16 * 10000) 
	char destinations[LENGTH_2] = {0}; 
	char content[LENGTH_1] = {0};
	char src_addr[LENGTH_1] = {0};
	int len1;
	time_t t1, t2;
	time(&t1);
	len1 = mg_get_var(conn, const_strings::DST_ADDRS.c_str(), destinations, LENGTH_2);
	len1 = mg_get_var(conn, const_strings::CONTENT.c_str(), content, LENGTH_1);
	len1 = mg_get_var(conn, const_strings::SRC_ADDR.c_str(), src_addr, LENGTH_1);
	if(strlen(destinations) > 0 && strlen(content) > 0)
	{
		std::vector<std::string> jsons;
		int count_of_batch = 200;
		int total = cb_client_format_sms(destinations, content, src_addr, count_of_batch, jsons);
		std::vector<std::string> session_ids;
		logger_log(__FUNCTION__, LEVEL_DEBUG, "receive: %d numbers, %d batches, src_addr:%s content:%s, destinations:%s", total, jsons.size(), src_addr, content, destinations);
		
		for(int i = 0; i < (int)jsons.size(); i++)
		{
			const std::string& json = jsons.at(i);
			std::string session_id = cb_client_submit_sms(json);
			if(session_id.length() == 0) 
			{
				logger_log(__FUNCTION__, LEVEL_WARNING, "submit failed, json: %s", json.c_str());
			}
			else
			{
				//logger_log(__FUNCTION__, LEVEL_DEBUG, "will submit, json:%s", json.c_str());
			}
			session_ids.push_back(session_id);
		}
 		std::string response = "";
		for(int i = 0; i < (int)session_ids.size(); i++)
		{
			if(session_ids.at(i).length() == 0)
			{
				response += "submit failed";
			}
			else
			{
				response += session_ids.at(i);
			}
			
			response += "<br>";
		}
		time(&t2);
		int coust_seconds = (int)(t2 - t1);
		if(coust_seconds >= 1)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "cost %d seconds, response:%s", coust_seconds, response.c_str());
		}
		else 
		{
			logger_log(__FUNCTION__, LEVEL_DEBUG, "response:%s", response.c_str());
		}
		
		response_200_plain_text(conn, response);
	}
	else
	{
		logger_log(__FUNCTION__, LEVEL_DEBUG, "invalid parameters, src_addr:%s content:%s, destinations:%s", src_addr, content, destinations);
		response_invalid_parameter(conn);
	}
	
	return MG_REQUEST_PROCESSED;
}
static int process_get_mos(struct mg_connection* conn)
{
	std::string json;
	cb_client_get_mos(json);
	mg_send_data(conn, json.c_str(), (int)json.length());
	return MG_REQUEST_PROCESSED;
}
static int process_get_dlrs(struct mg_connection* conn)
{
	std::string json;
	cb_client_get_dlrs(100, json);
	mg_send_data(conn, json.c_str(), (int)json.length());
	return MG_REQUEST_PROCESSED;
}

static void process_get_clients_statistic(struct mg_connection* conn)
{
	std::string statistic;
	cb_client_get_statistic_string(statistic);
	response_200_full_html(conn, statistic);
}


static int request_handler(struct mg_connection *conn)
{
	if(get_submit.compare(conn->uri) == 0)
	{
		return process_get_submit(conn);
	}
	else if(get_mos.compare(conn->uri) == 0)
	{
		process_get_mos(conn);
		return MG_REQUEST_PROCESSED;
	}
	else if(get_balance.compare(conn->uri) == 0)
	{
		process_get_balance(conn);
		return MG_REQUEST_PROCESSED;
	}
	else if(get_dlrs.compare(conn->uri) == 0)
	{
		process_get_dlrs(conn);
		return MG_REQUEST_PROCESSED;
	}

	else if(get_statistic.compare(conn->uri) == 0)
	{
		process_get_clients_statistic(conn);
		return MG_REQUEST_PROCESSED;
	}
	else
	{
		return MG_REQUEST_NOT_PROCESSED;
	}
}

void* process_proc(void* p_server)
{
	while(!stop_webserver)
	{
		mg_poll_server((struct mg_server*)p_server, 500);
	}
	return NULL;
}

bool start_simple_webserver(const std::string& port)
{
	if(port.length() > 0)
	{
		s_port = port;
	}
	logger_log(__FUNCTION__, LEVEL_INFO, "web server started, listen port is: %s", s_port.c_str());
	stop_webserver = false;
	for(int i = 0; i < MAX_THREADS; i++)
	{
		server[i] = mg_create_server(NULL);
		if(i == 0)
		{
			mg_set_option(server[i], "listening_port", s_port.c_str());
		}
		else
		{
			mg_set_listening_socket(server[i], mg_get_listening_socket(server[0]));
		}
		mg_set_option(server[i], "document_root", s_root_fullpath.c_str());
		mg_set_request_handler(server[i], request_handler);
		mg_start_thread(process_proc, server[i]);
	}

	return true;
}

void stop_simple_webserver()
{
	stop_webserver = true;
	for(int i = 0; i < MAX_THREADS; i++)
	{
		mg_destroy_server(&server[i]);
	}
}

