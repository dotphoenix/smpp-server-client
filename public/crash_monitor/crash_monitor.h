#ifndef CRASH_MONITOR_H
#define CRASH_MONITOR_H
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp> 
#include <boost/interprocess/managed_shared_memory.hpp>
#include "logger.hpp"
#include "boost/process.hpp"

#define CB_CRASH_MONITOR_SHARED_MEMORY "CBDaemon-SM"
#define CB_CRASH_MONITOR_SHARED_MEMORY_LOCK_NAME "CBDaemon-SM-Lock"

#define CHILD_PROCESS_NAME_1 "./SmppServer"
#define CHILD_PROCESS_NAME_2 "./ICBCPlatform"
#define CHILD_PROCESS_NAME_DAEMON "./Daemon"

//#define CB_DAEMON

struct crash_monitor_process_info
{
	char process_path[1024];
	boost::uint64_t update_time;
};
struct cb_shm_remover
{
protected:
	std::string sm_name_;
	void remove_if(const char* from)
	{
		bool b = boost::interprocess::shared_memory_object::remove(sm_name_.c_str());
		if(b)
		{
			logger_log(__FUNCTION__, LEVEL_DEBUG, "remove sm %s successfully(%s)", sm_name_.c_str(), from);
		}
		else
		{
			logger_log(__FUNCTION__, LEVEL_DEBUG, "remove sm %s failed(%s)", sm_name_.c_str(), from);
		}
	}
public:
	cb_shm_remover(const std::string& sm_name)
	{
		sm_name_ = sm_name;
		remove_if("constructor");
	}
	~cb_shm_remover()
	{
		remove_if("destructor");
	}
};
struct cb_name_mutex_remover
{
protected:
	std::string mutex_name_;
	void remove_if(const char* from)
	{
		bool b = boost::interprocess::named_mutex::remove(mutex_name_.c_str());
		if(b)
		{
			logger_log(__FUNCTION__, LEVEL_DEBUG, "remove name mutex %s successfully(%s)", mutex_name_.c_str(),
					   from);
		}
		else
		{
			logger_log(__FUNCTION__, LEVEL_DEBUG, "remove name mutex %s failed(%s)", mutex_name_.c_str(),
					   from);
		}
	}
public:
	cb_name_mutex_remover(const std::string& mutex_name)
	{
		mutex_name_ = mutex_name;
		remove_if("constructor");
	}
	~cb_name_mutex_remover()
	{
		remove_if("destructor");
	}
};
extern std::string crash_monitor_get_child_app_path();
extern void* crash_monitor_sm_start_address;


inline boost::process::child crash_monitor_start_child(const std::string& exec, const std::vector<std::string>& args)
{
	std::vector<std::string> args1;
	args1.push_back(exec);
	for(size_t i = 0; i < args.size(); i++)
	{
		args1.push_back(args.at(i));
	}
	boost::process::context ctx;
	ctx.stdout_behavior = boost::process::capture_stream();
	
	boost::process::child c = boost::process::launch(exec, args1, ctx);
	logger_log(__FUNCTION__, LEVEL_DEBUG, "create child process: %s, pid is %u", exec.c_str(), (unsigned int)c.get_id());
	return c;
}

inline void crash_monitor_init_children_info()
{
	int total_children_nb = 0;
	cb_name_mutex_remover mutex_remover(CB_CRASH_MONITOR_SHARED_MEMORY_LOCK_NAME);
#ifdef CB_DAEMON
	crash_monitor_process_info* pi_child_1 = NULL;
	crash_monitor_process_info* pi_child_2 = NULL;
	
	total_children_nb = 2;
	memcpy(crash_monitor_sm_start_address, &total_children_nb, sizeof(int));
	
	pi_child_1 = (crash_monitor_process_info*)((unsigned char*)crash_monitor_sm_start_address + sizeof(int));
	strcpy(pi_child_1->process_path, CHILD_PROCESS_NAME_1);
	pi_child_1->update_time = (boost::uint64_t)time(NULL);
	
	pi_child_2 = (crash_monitor_process_info*)((unsigned char*)crash_monitor_sm_start_address + sizeof(int) + sizeof(crash_monitor_process_info));
	strcpy(pi_child_2->process_path, CHILD_PROCESS_NAME_2);
	pi_child_2->update_time = (boost::uint64_t)time(NULL);
	
#else
	crash_monitor_process_info* pi_child_me = NULL;
	
	total_children_nb = 1;
	memcpy(crash_monitor_sm_start_address, &total_children_nb, sizeof(int));
	
	pi_child_me = (crash_monitor_process_info*)((unsigned char*)crash_monitor_sm_start_address + sizeof(int));
	strcpy(pi_child_me->process_path, CHILD_PROCESS_NAME_DAEMON);
	pi_child_me->update_time = (boost::uint64_t)time(NULL);
#endif
}
inline int crash_monitor_get_children_nb()
{
	int total_children;
	memcpy(&total_children, crash_monitor_sm_start_address, sizeof(total_children));
	return total_children;
}
inline crash_monitor_process_info* crash_monitor_get_child_info(int index)
{
	if((index < 0) || (index >= crash_monitor_get_children_nb()))
	{
		return NULL;
	}
	crash_monitor_process_info* pi = (crash_monitor_process_info*)((unsigned char*)crash_monitor_sm_start_address + sizeof(int) + index * sizeof(crash_monitor_process_info));
	return pi;
}
inline void crash_monitor_start_children()
{
	int children_nb = crash_monitor_get_children_nb();
	boost::interprocess::named_mutex sm_lock(boost::interprocess::open_or_create, CB_CRASH_MONITOR_SHARED_MEMORY_LOCK_NAME);
	sm_lock.lock();
	for(int i = 0; i < children_nb; i++)
	{
		boost::uint64_t now = (boost::uint64_t)time(NULL);
		crash_monitor_process_info* pi = crash_monitor_get_child_info(i);
		if(pi == NULL)
		{
			continue;
		}
		if(strlen(pi->process_path) == 0)
		{
			continue;
		}

		pi->update_time = now;
		std::vector<std::string> args;
#ifdef CB_DAEMON
			
#else
		args.push_back("-c");
#endif
		crash_monitor_start_child(pi->process_path, args);
	}
	sm_lock.unlock();
}
inline void crash_monitor_check_child_and_start()
{
	int children_nb = crash_monitor_get_children_nb();
	for(int i = 0; i < children_nb; i++)
	{
		crash_monitor_process_info* pi = crash_monitor_get_child_info(i);
		if(pi == NULL)
		{
			break;
		}
		if(strlen(pi->process_path) == 0)
		{
			break;
		}
		boost::uint64_t now = (boost::uint64_t)time(NULL);
		boost::uint64_t update_time = now;
		boost::interprocess::named_mutex sm_lock(boost::interprocess::open_or_create, CB_CRASH_MONITOR_SHARED_MEMORY_LOCK_NAME);
		sm_lock.lock();
		update_time = pi->update_time;
		if(now - update_time > 30)
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "%s is not alive, we will create it", pi->process_path);
			pi->update_time = now;
			std::vector<std::string> args;
#ifdef CB_DAEMON
			
#else
			args.push_back("-c");
#endif
			crash_monitor_start_child(pi->process_path, args);
		}
		sm_lock.unlock();
	}
}
inline bool crash_monitor_child_update_time()
{
	static int count = 0;
	crash_monitor_process_info* child = NULL;
	std::string process_path = crash_monitor_get_child_app_path();
	int total_children_nb = crash_monitor_get_children_nb();
	for(int i = 0; i < total_children_nb; i++)
	{
		crash_monitor_process_info* p = (crash_monitor_process_info*)((unsigned char*)crash_monitor_sm_start_address + sizeof(int) + i * sizeof(crash_monitor_process_info));
		if(strcmp(p->process_path, process_path.c_str()) == 0)
		{
			child = p;
			break;
		}
	}
	if(child == NULL)
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "child: %s not exist in shared memory", process_path.c_str());
		return false;
	}
	boost::uint64_t now = (boost::uint64_t)time(NULL);
	boost::interprocess::named_mutex sm_lock(boost::interprocess::open_or_create, CB_CRASH_MONITOR_SHARED_MEMORY_LOCK_NAME);
	sm_lock.lock();
	child->update_time = now;
	sm_lock.unlock();
	count++;
	if(count > 30)
	{
		logger_log(__FUNCTION__, LEVEL_DEBUG, "child update time");
		count = 0;
	}
	
	return true;
}
#endif
