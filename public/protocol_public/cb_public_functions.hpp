#ifndef CB_PUBLIC_FUNCTIONS_
#define CB_PUBLIC_FUNCTIONS_
#include <string>
#include "memory_buffer.hpp"
bool is_bit_1(unsigned char& byte, int bit);

bool get_file_length(const std::string& fullpath, size_t& length);
bool read_binay_file(const std::string& full_path, memory_buffer_ptr& bp);
bool write_binary_file(const std::string& full_path, memory_buffer_ptr bp);
bool get_filename_detail(const std::string& full_path, std::string& filename, std::string& extension);

#endif