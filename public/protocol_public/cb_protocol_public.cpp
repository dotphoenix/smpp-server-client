#include "algorithm.hpp"
#include "logger.hpp"
#include <string.h>
#include <boost/thread/mutex.hpp>
#include <limits.h>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include "cb_public_functions.hpp"
#include "cb_protocol_public.hpp"


std::string generate_message_id()
{
	unsigned int n = generate_sn();
	char hex[16] = {0};
	sprintf(hex, "%x", n);
	std::string s = hex;
	boost::to_upper(s);
	return s;
}
void generate_message_id_tester()
{
	std::list<std::string> l;
	for(int i = 0; i < 1000000; i++)
	{
		l.push_back(generate_message_id());
	}
	size_t before = l.size(), after;
	l.sort();
	l.unique();
	after = l.size();
	l.clear();
	std::cout<<"befor: "<<before<<", after: "<<after<<std::endl;
}
boost::mutex generate_sn_mutex_;
static unsigned int sn_magic_number = 0;
static unsigned int get_sn_magic_number()
{
	if(sn_magic_number == 0)
	{
		typedef boost::mt19937 RNGType;
		srand((unsigned int)time(NULL));
		RNGType rng(rand() % 777);
		boost::uniform_int<> one_to_max_int( 1, INT_MAX>>3);
		boost::variate_generator< RNGType, boost::uniform_int<> >
		dice(rng, one_to_max_int);
		sn_magic_number = dice();
	}
	return sn_magic_number;
}
unsigned int generate_sn()
{
	boost::mutex::scoped_lock lock(generate_sn_mutex_);
	static int i = 111;
	unsigned int n = get_sn_magic_number() + i++;
	if(i > 999999999)
	{
		i = 111;
	}
	return n;
}
