//
// logger.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2013 Zhu Nengjie (Bifang technology)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string>
#include <fstream>

#ifndef WIN32
#include <pthread.h>
#else
//#include <winsock2.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
typedef HANDLE pthread_mutex_t;
typedef struct {HANDLE signal, broadcast;} pthread_cond_t;
typedef DWORD pthread_t;
#define pid_t HANDLE // MINGW typedefs pid_t to int. Using #define here.

inline int pthread_mutex_init(pthread_mutex_t *mutex, void *unused) {
	unused = NULL;
	*mutex = CreateMutex(NULL, FALSE, NULL);
	return *mutex == NULL ? -1 : 0;
}

inline  int pthread_mutex_destroy(pthread_mutex_t *mutex) {
	return CloseHandle(*mutex) == 0 ? -1 : 0;
}

inline  int pthread_mutex_lock(pthread_mutex_t *mutex) {
	return WaitForSingleObject(*mutex, INFINITE) == WAIT_OBJECT_0? 0 : -1;
}

inline  int pthread_mutex_unlock(pthread_mutex_t *mutex) {
	return ReleaseMutex(*mutex) == 0 ? -1 : 0;
}

inline  int pthread_cond_init(pthread_cond_t *cv, const void *unused) {
	unused = NULL;
	cv->signal = CreateEvent(NULL, FALSE, FALSE, NULL);
	cv->broadcast = CreateEvent(NULL, TRUE, FALSE, NULL);
	return cv->signal != NULL && cv->broadcast != NULL ? 0 : -1;
}

inline  int pthread_cond_wait(pthread_cond_t *cv, pthread_mutex_t *mutex) {
	HANDLE handles[] = {cv->signal, cv->broadcast};
	ReleaseMutex(*mutex);
	WaitForMultipleObjects(2, handles, FALSE, INFINITE);
	return WaitForSingleObject(*mutex, INFINITE) == WAIT_OBJECT_0? 0 : -1;
}

inline  int pthread_cond_signal(pthread_cond_t *cv) {
	return SetEvent(cv->signal) == 0 ? -1 : 0;
}

inline  int pthread_cond_broadcast(pthread_cond_t *cv) {
	// Implementation with PulseEvent() has race condition, see
	// http://www.cs.wustl.edu/~schmidt/win32-cv-1.html
	return PulseEvent(cv->broadcast) == 0 ? -1 : 0;
}

inline  int pthread_cond_destroy(pthread_cond_t *cv) {
	return CloseHandle(cv->signal) && CloseHandle(cv->broadcast) ? 0 : -1;
}

#endif
/*
the library current support logging to file, console, callback and tcp socket(this is a client, but not 
including a tcp server in this file, you can search "boost asio tcp server" on google to get one easily)

for easy to use, the library including a class logger and its c wrapper(logger_init, logger_uninit, logger_log , etc.);
you can refer the function  logger_tester() in logger.cpp to learn how to use the c wrapper apis, you also
can use the logger class directly, in this way, you should refer the c wrapper apis, they wrapper the class.

if you find any bugs or need any help, please contact with me(Bifang technology)
*/

enum LEVEL
{
	LEVEL_FATAL ,
	LEVEL_ERROR ,
	LEVEL_WARNING ,
	LEVEL_DEBUG ,
	LEVEL_INFO ,
	LEVEL_NONE
};

typedef void (*log_function)(const char* message);
class logger
{
public:
	enum DESTINATION
	{
		DESTINATION_NONE = 0,
		DESTINATION_CONSOLE = 0x1,
		DESTINATION_FILE = 0x2,
		DESTINATION_CALLBACK = 0x4,
		DESTINATION_SOCKET = 0x8
	};
protected:
	logger();
	~logger();
	static logger* instance_;
	pthread_mutex_t trace_lock;
public:
	static logger* get_instance()
	{
		if(instance_ == NULL)
		{
			instance_ = new logger();
		}
		return instance_;
	}
	static void free_instance()
	{
		if(instance_ != NULL)
		{
			delete instance_;
			instance_ = NULL;
		}
	}
public:
	std::string get_path();
	void set_filename(const std::string& filename);
	void set_callback(log_function fun);
	void set_logserver(const std::string& logserver_ip, unsigned short logserver_port);	
	void set_level(LEVEL show_level){show_level_ = show_level;}
	void set_destination(DESTINATION dest){destination_ = dest;}

	//void log(const char* domain,  LEVEL level, const char* format, ...);

	void log(const char* domain, LEVEL level, const std::string& msg);
	void log(const std::string& msg);
private:
	std::string filename_;
	std::ofstream file_;
	log_function callback_;
	std::string logserver_ip_;
	unsigned short logserver_port_;
	DESTINATION destination_;
	LEVEL show_level_;

	void close_socket_if_connected();
	void close_file_if_opened();
public:
	static std::string current_time();
};

/*
********************below are c wrapper apis*************************8
*/
//log to console
void logger_init(LEVEL show_level);

//log to file
void logger_init(LEVEL show_level, const std::string& filename);

//log to a tcp server 
// protocol is : msg_length(4 bytes) + msg
void logger_init(LEVEL show_level, const std::string& ip, unsigned short port);

//void log to callback
void logger_init(LEVEL show_level, log_function callback);

//show msg
void logger_log(const char* domain, LEVEL level, const char* format, ...);
void logger_log(const char* format, ...);
//release resource
void logger_uninit();

// change show level dynamically
void logger_set_level(LEVEL show_level);

//change log to callbakc
void logger_set_callback(log_function callback);

//change log to file
void logger_set_filename(const std::string& filename);

//change log to tcp server
void logger_set_logserver(const std::string& logserver_ip, unsigned short logserver_port);

// a tool function used to return the path of the file of the calling process
std::string logger_get_path();

/*
#ifndef min
#define min(x, y) ((x) <= (y) ? (x) : (y))
#endif

#ifndef max
#define max(x, y) ((x) >= (y) ? (x) : (y))
#endif
*/
#endif

