#include "algorithm.hpp"
#include "cb_public_functions.hpp"
#include <boost/thread/mutex.hpp>
#include "base64.h"
#include <sys/stat.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include "cb_protocol_public.hpp"
bool is_bit_1(unsigned char& byte, int bit)
{
	if((bit >=8) || (bit < 0))
	{
		return false;
	}
	return (((byte >> bit) & 1) == 1);
}



bool get_file_length(const std::string& fullpath, size_t& length)
{
	struct stat file_state;
	if (stat(fullpath.c_str(), &file_state) == 0)
	{
		length = file_state.st_size;
		return true;
	}
	return false;
}
bool read_binay_file(const std::string& full_path, memory_buffer_ptr& bp)
{
	std::ifstream f(full_path.c_str(), std::ios::in | std::ios::binary);
	if(f.is_open())
	{
		size_t len;
		if(!get_file_length(full_path, len))
		{
			return false;
		}
		bp.reset(new memory_buffer(len));
		f.read((char*)bp->data(), len);
		f.close();
		return true;
	}
	return false;
}
bool write_binary_file(const std::string& full_path, memory_buffer_ptr bp)
{
	std::ofstream f(full_path.c_str(), std::ios::out | std::ios::binary);
	if(f.is_open())
	{
		size_t size = bp->size();
		f.write((char*)bp->data(), size);
		f.close();
		return true;
	}
	else
	{
		return false;
	}
}

bool get_filename_detail(const std::string& full_path, std::string& filename, std::string& extension)
{
	boost::filesystem::path path = full_path;
	filename = path.stem().string();
	extension = path.extension().string();
	return true;
}

#ifdef TEST_STL
#define MAX_TEST_DATA (1000000)
struct A
{
	unsigned int i;
	std::string uuid;
	std::string s1;
	unsigned int i3;
	bool operator<(const A& a) const
	{
		return uuid.compare(a.uuid) < 0;
	}
	bool operator==(const A& a) const
	{
		return uuid.compare(a.uuid) == 0;
	}
	bool operator==(const std::string& s) const
	{
		return uuid.compare(s) == 0;
	}
	int compare(const A& other)
	{
		return uuid.compare(other.uuid);
	}
};



void test_list()
{
	std::list<A> la;
	A to_find;
	std::vector<A> va;
	{
		WORK_START
		for(size_t i = 0; i < MAX_TEST_DATA; i++)
		{
			A a;
			a.uuid = generate_message_id();
			va.push_back(a);
		}
		WORK_FINISH("generate 100,0000 uuid and push to vector")
	}
	{
		WORK_START
		for(size_t i = 0; i < MAX_TEST_DATA; i++)
		{
			la.push_back(va.at(i));
			if(i == (MAX_TEST_DATA * 3 / 4))
			{
				to_find = va.at(i);
			}
		}
		WORK_FINISH("generate 100,0000 uuid and push back to list")
	}
	{
		// get size
		WORK_START
		la.size();
		WORK_FINISH("get size of list (100, 0000)");
	}
	{
		WORK_START
		std::find(la.begin(), la.end(), to_find);
		WORK_FINISH("find uuid in 100,0000 uuids ")
	}

	{
		//sort
		WORK_START
		la.sort();
		WORK_FINISH("sort 100,0000 uuid ")
	}

	{
		WORK_START
		std::binary_search(la.begin(), la.end(), to_find);
		WORK_FINISH("binary search 100,0000 uuids")
	}
}

void test_hash_map()
{

}

void test_stl()
{
	test_list();
	test_hash_map();
}

#endif