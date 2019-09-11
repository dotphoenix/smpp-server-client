//
// logger.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2013 Zhu Nengjie (Bifang technology)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#ifndef WIN32
#include <sys/time.h>
#else
#include <Windows.h>
#endif
#include "logger.hpp"

#ifdef ANDROID
extern void android_log_handler(const char* domain, LEVEL lev, const char *msg);
#endif

logger* logger::instance_ = NULL;

logger::logger():callback_(NULL),destination_(DESTINATION_CONSOLE),show_level_(LEVEL_WARNING)
		
{
	pthread_mutex_init(&trace_lock, NULL);
}
logger::~logger()
{
	show_level_ = LEVEL_NONE;
	destination_ = DESTINATION_NONE;
	close_file_if_opened();
	close_socket_if_connected();
	callback_ = NULL;
	pthread_mutex_destroy(&trace_lock);
}
void logger::close_file_if_opened()
{
	if(file_.is_open())
	{
		file_.flush();
		file_.close();
	}
}
void logger::close_socket_if_connected()
{

}

std::string logger::current_time()
{
	char tmp[128];
	
#ifdef WIN32
	SYSTEMTIME sys;
	memset(tmp, 0, 128);
	GetLocalTime(&sys);
	sprintf(tmp, "%02d/%02d %02d:%02d:%02d.%03d", sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds); 
#else
	struct timeval tv;
	time_t curtime;
	struct tm* t;
	gettimeofday(&tv, NULL); 
	curtime=tv.tv_sec;
	t = localtime(&curtime);
	sprintf(tmp, "%02d/%02d %02d:%02d:%02d.%03d",t->tm_mon + 1, t->tm_mday, 
			t->tm_hour, t->tm_min, t->tm_sec, (int)(tv.tv_usec / 1000)); 
#endif
	return std::string(tmp);
}
void logger::set_filename(const std::string& filename)
{
	destination_ = DESTINATION_FILE;
	filename_ = filename;
	close_file_if_opened();
	file_.open(filename_.c_str(), std::ios_base::out | std::ios_base::app);
	
}
void logger::set_callback(log_function fun)
{
	destination_ = DESTINATION_CALLBACK;
	callback_ = fun;
}
void logger::set_logserver(const std::string& logserver_ip, unsigned short logserver_port)
{

}
/*
void logger::log(const char* domain,  LEVEL level, const char* format, ...)
{
	va_list arg_ptr;
	va_start(arg_ptr, format);
	#ifdef ANDROID
	if(!file_.is_open())
	{
		android_log_handler(domain,  level, format, arg_ptr);
		void android_log_handler(const char* domain, LEVEL lev, const char *msg)
		return;
	}
	#endif
	std::string smsg;
	char msg[4096] = {0};
	vsnprintf(msg, 4096 - 2, format, arg_ptr);
	smsg = msg;
	log(domain, level, smsg);
	va_end(arg_ptr);
}*/
void logger::log(const char* domain, LEVEL level, const std::string& msg)
{
#ifdef ANDROID
	if(!file_.is_open())
	{
		android_log_handler(domain, level, msg.c_str());
		return;
	}
#endif
	
	if(level <= show_level_)
	{
		std::string slevel;
		switch(level)
		{
		case LEVEL_INFO:
			slevel = "<Info> ";
			break;
		case LEVEL_DEBUG:
			slevel = "<Debug> ";
			break;
		case LEVEL_WARNING:
			slevel = "<Warning> ";
			break;
		case LEVEL_ERROR:
			slevel = "<Error> ";
			break;
		case LEVEL_FATAL:
			slevel = "<Fatal> ";
			break;
		default:
			slevel = "<None>";
			break;
		}

		std::stringstream stream;
		stream<<"["<<domain<<"]"<<slevel
			<<"["<<current_time()<<"]"
			<<" "<<msg.c_str()<<"\n";
		
		std::string s = stream.str();
		log(s);
	}
}

void logger::log(const std::string& msg)
{
	pthread_mutex_lock(&trace_lock);
	switch(destination_)
	{
		case DESTINATION_CONSOLE:
		{
#if defined _DEBUG && defined WIN32
			OutputDebugStringA(msg.c_str());
#endif
		 	std::cout<<msg<<std::endl;
			break;
		}
		case DESTINATION_FILE:
		{
			if(file_.is_open())
			{
				file_<<msg;
				file_.flush();
			}
			break;
		}
		case DESTINATION_SOCKET:
		{
			break;
		}
		case DESTINATION_CALLBACK:
		{
			if(callback_ != NULL)
			{
				callback_(msg.c_str());
			}
			break;
		}
		default:
			break;
	}
	pthread_mutex_unlock(&trace_lock);
}
void logger_init(LEVEL show_level)
{
	logger::get_instance()->set_level(show_level);
}
void logger_init(LEVEL show_level, const std::string& filename)
{
	logger::get_instance()->set_level(show_level);
	logger::get_instance()->set_filename(filename);
}
void logger_init(LEVEL show_level, const std::string& logserver_ip, unsigned short logserver_port)
{
	logger::get_instance()->set_level(show_level);
	logger::get_instance()->set_logserver(logserver_ip, logserver_port);
}
void logger_init(LEVEL show_level, log_function callback)
{
	logger::get_instance()->set_level(show_level);
	logger::get_instance()->set_callback(callback);
}
#define BUFFER_SIZE (1024 * 128)
void logger_log(const char* domain, LEVEL level, const char* format, ...)
{
	va_list arg_ptr;
	va_start(arg_ptr, format);
	std::string smsg;
	char msg[BUFFER_SIZE] = {0};
	vsnprintf(msg, BUFFER_SIZE - 2, format, arg_ptr);
	logger::get_instance()->log(domain, level, std::string(msg));
	va_end(arg_ptr);
}
void logger_log(const char* format, ...)
{
	va_list arg_ptr;
	va_start(arg_ptr, format);
	std::string smsg;
	char msg[BUFFER_SIZE] = {0};
	vsnprintf(msg, BUFFER_SIZE - 2, format, arg_ptr);
	logger::get_instance()->log("nuknow function", LEVEL_DEBUG, std::string(msg));
	va_end(arg_ptr);

}
void logger_uninit()
{
	logger::get_instance()->free_instance();
}

void logger_set_level(LEVEL show_level)
{
	logger::get_instance()->set_level(show_level);
}
void logger_set_callback(log_function callback)
{
	logger::get_instance()->set_callback(callback);
}
void logger_set_filename(const std::string& filename)
{
	logger::get_instance()->set_filename(filename);
}
void logger_set_logserver(const std::string& logserver_ip, unsigned short logserver_port)
{
	logger::get_instance()->set_logserver(logserver_ip, logserver_port);
}	


void logger_tester_callback(const char* msg)
{
	fprintf(stdout, "%s ", (char*)msg);
}

/*
I did not implement a tcp server here, you can go to www.boost.org/docs to get a normal tcp server(search boost
asio tcp server on google), or you also can contact with me(Bifang technology), I will help you.
*/
void logger_tester_tcpserver_start()
{

}
void logger_tester_tcpserver_stop()
{

}
std::string logger_get_path()
{	
	return "./";
}
void logger_tester()
{
	logger_init(LEVEL_DEBUG);
	logger_log("console test", LEVEL_WARNING, "console test warning");
	logger_log("console test", LEVEL_DEBUG, "console test debug");
	logger_log("console test", LEVEL_INFO, "console test info");

	std::string log_file = logger_get_path() + "log_file.log";
	logger_set_filename(log_file);
	logger_log("file test", LEVEL_WARNING, "file test warning");
	logger_log("file test", LEVEL_DEBUG, "file test debug");
	logger_log("file test", LEVEL_INFO, "file test info");

	logger_set_callback(logger_tester_callback);
	logger_set_level(LEVEL_WARNING);
	logger_log("callback test", LEVEL_WARNING, "callback test warning");
	logger_log("callback test", LEVEL_DEBUG, "callback test debug");
	logger_log("callback test", LEVEL_INFO, "callback test info");

	logger_uninit();

	std::string ip = "127.0.0.1";
	unsigned short port = 9980;
	logger_init(LEVEL_DEBUG, ip, port);
	logger_log("tcp test", LEVEL_WARNING, "tcp test warning");
	logger_log("tcp test", LEVEL_DEBUG, "tcp test debug");
	logger_log("tcp test", LEVEL_INFO, "tcp test info");
	logger_uninit();
}

void logger_warning(const char* domain, const char* format, ...)
{
	va_list arg_ptr;
	static char msg[4096] = {0};
	va_start(arg_ptr, format);
	vsnprintf(msg, 4096 - 2, format, arg_ptr);
	va_end(arg_ptr);
	logger::get_instance()->log(domain, LEVEL_WARNING, std::string(msg));
}
void logger_debug(const char* domain, const char* format, ...)
{
	va_list arg_ptr;
	static char msg[4096] = {0};
	va_start(arg_ptr, format);
	vsnprintf(msg, 4096 - 2, format, arg_ptr);
	va_end(arg_ptr);
	logger::get_instance()->log(domain, LEVEL_DEBUG, std::string(msg));
}
void logger_error(const char* domain, const char* format, ...)
{
	va_list arg_ptr;
	static char msg[4096] = {0};
	va_start(arg_ptr, format);
	vsnprintf(msg, 4096 - 2, format, arg_ptr);
	va_end(arg_ptr);
	logger::get_instance()->log(domain, LEVEL_ERROR, std::string(msg));
}
