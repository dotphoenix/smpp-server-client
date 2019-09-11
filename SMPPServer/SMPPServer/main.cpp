#include "algorithm.hpp"
#include <algorithm>
#include <cstdlib>
#include <string>
#include "smppserver_runtime.h"
#include <list>
#include "configer.hpp"

void print_copyright()
{
	std::string version = "V1.0";
	std::string copyright = "SMPPServer ";
	copyright = copyright + version + "  Build at:" + __DATE__;
	copyright = copyright + "\nCopyright (C) 2015 - 2018, Bifang technology, All right reserved.";
	logger_log(__FUNCTION__, LEVEL_WARNING, "%s", copyright.c_str());
}
std::string get_config_file_path()
{
	boost::filesystem::path rootpath = boost::filesystem::initial_path<boost::filesystem::path>();
	boost::filesystem::path configerpath = rootpath ;
	configerpath += boost::filesystem::path::preferred_separator;
	configerpath += "configer/smppserver_configer.ini";
	return configerpath.string();
}
void read_log_config(bool& to_console, LEVEL& level)
{
	std::string config_file = get_config_file_path();
	if(boost::filesystem::exists(config_file))
	{
		configer conf(config_file);
		std::string sec_name = "log";
		to_console = (conf.get_int(sec_name, "console", 1) == 1);
		level = (LEVEL)(conf.get_int(sec_name, "level", 3));//3 is debug, 2 is warning
	}
	else
	{
		to_console = true;
		level = LEVEL_DEBUG;
	}
}
static void sig_int(int num) 
{
	smppserver_runtime::get_instance()->stop_smppserver();
	logger_log(__FUNCTION__, LEVEL_WARNING, "program will stopped.");
    return; 
}
extern void mysql_client_helper_performance_tester();
int main(int argc, char* argv[])
{
	signal(SIGINT, sig_int);
	bool to_console;
	LEVEL level;
	read_log_config(to_console, level);
	if(to_console)
	{
		logger_init(level);
	}
	else
	{
		std::string logfile = logger_get_path() + "cb_smppserver.log";
		logger_init(level, logfile);
	}
	print_copyright();
	smppserver_runtime::get_instance()->run_smppserver();
	logger_log(__FUNCTION__, LEVEL_INFO, "SMPP server stopped");
	logger_uninit();
	return 0;
}
