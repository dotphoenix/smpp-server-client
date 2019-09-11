#ifndef MEMORY_BUFFER_H
#define MEMORY_BUFFER_H
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <iostream>
#include <string.h>
class memory_buffer
{
public:
	memory_buffer(std::size_t size ):impl_(size){memset((void*)impl_.data(), 0, size);}
	~memory_buffer(){}
	void resize(std::size_t new_size){impl_.resize(new_size);}
	const unsigned char* data(){return &impl_[0];}
	std::size_t size(){return impl_.size();}
	void set_data(int begin_index, void* data, size_t data_len)
	{
		memcpy((void*)(impl_.data() + begin_index), data, data_len);
	}
	void copy_data(int begin_index, void* data, size_t data_len)
	{
		memcpy(data, (void*)(impl_.data() + begin_index), data_len);
	}
protected:
	std::vector<unsigned char> impl_;
};
typedef boost::shared_ptr<memory_buffer> memory_buffer_ptr;
typedef boost::optional<memory_buffer_ptr> opt_bffr_ptr;

inline void memory_buffer_ptr_tester()
{
	memory_buffer_ptr bp;
	std::cout<<"After: memory_buffer_ptr bp;  --->";
	if(bp.get() == NULL)
	{
		std::cout<<"bp.get() returns null"<<std::endl;
	}
	else
	{
		std::cout<<"bp.get() returns not null"<<std::endl;
	}
	bp.reset(new memory_buffer(1));
	std::cout<<"After bp.reset(new memory_buffer(1));  --->";
	if(bp.get() == NULL)
	{
		std::cout<<"bp.get() returns null"<<std::endl;
	}
	else
	{
		std::cout<<"bp.get() returns not null"<<std::endl;
	}
	bp.reset();
	std::cout<<"After bp.reset();  --->";
	if(bp.get() == NULL)
	{
		std::cout<<"bp.get() returns null"<<std::endl;
	}
	else
	{
		std::cout<<"bp.get() returns not null"<<std::endl;
	}
}
#endif