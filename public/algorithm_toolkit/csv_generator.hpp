#ifndef CSV_GENERATOR_H
#define  CSV_GENERATOR_H
#include <string>
#include <vector>
class csv_generator
{
public:
	static void generator(const std::string& path, const std::vector< std::vector<std::wstring> >& wlines );
	static bool parse(const std::string& path, std::vector< std::vector<std::string> >& lines );
	static void tester( );
};
#endif