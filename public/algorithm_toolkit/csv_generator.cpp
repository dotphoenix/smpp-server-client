#include "csv_generator.hpp"
#include <sstream>
#include "algorithm.hpp"
static void write_wline(std::wofstream& wofs, const std::wstring& wline)
{
#ifdef WIN32
	std::wstring CRLF = L"\n";
#elif defined __APPLE__
	std::wstring CRLF = L"\r";
#else
	std::wstring CRLF = L"\n";
#endif
	wofs<<wline;
	wofs<<CRLF;
	wofs.flush();
}
static bool read_wline(std::wifstream& wifs, std::wstring& wline)
{
	if(getline(wifs, wline))
	{
		return true;
	}
	return false;
}
void csv_generator::generator(const std::string& path, const std::vector< std::vector<std::wstring> >& wlines )
{
	std::wofstream wofs;
#if defined WIN32
	std::locale loc("");
	std::locale::global(loc);//cht(windows) or zh_TW.BIG5(Linux) for chinese traditional
	wofs.open(s2ws(path));
#else
	std::locale loc("");
	std::locale::global(loc);//cht(windows) or zh_TW.BIG5(Linux) for chinese traditional
	wofs.open(path.c_str());
#endif
	if(!wofs.is_open())
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "create csv file %s failed.", path.c_str());
		return ;
	}
	logger_log(__FUNCTION__, LEVEL_DEBUG, "create csv file %s successfully.", path.c_str());
	for(size_t i = 0; i < wlines.size(); i++)
	{
		const std::vector<std::wstring>& line = wlines.at(i);
		std::wstringstream wss;
		for(size_t j = 0; j < line.size(); j++)
		{
			if(j > 0)
			{
				wss<<L",";
			}
			wss<<line.at(j);
		}
		wofs.imbue(loc);
		write_wline(wofs, wss.str());
	}
	wofs.close();
}
bool csv_generator::parse(const std::string& path, std::vector< std::vector<std::string> >& lines )
{
	std::wifstream wifs;
#if defined WIN32
	wifs.open(s2ws(path));
#else
	std::locale::global(std::locale("chs"));//cht(windows) or zh_TW.BIG5(Linux) for chinese traditional
	wifs.open(path.c_str());
#endif
	if(!wifs.is_open())
	{
		logger_log(__FUNCTION__, LEVEL_WARNING, "open csv file %s failed.", path.c_str());
		return false;
	}
	logger_log(__FUNCTION__, LEVEL_DEBUG, "open csv file %s successfully.", path.c_str());
	std::wstring wline= L"";
	wifs.imbue(std::locale("chs")); //cht(windows) or zh_TW.BIG5(Linux) for Chinese traditional
	while(read_wline(wifs, wline))
	{
		std::vector<std::string> vw;
		std::string splitter = ",";
		std::string line = ws2s(wline);
		boost::split(vw, line, boost::algorithm::is_any_of(splitter), boost::algorithm::token_compress_off);
		lines.push_back(vw);
	}
	wifs.close();
	return true;
}
#ifdef WIN32
#include <tchar.h>
#else
#define _T(X) L##X
#endif
#include <wchar.h>
void csv_generator::tester( )
{
	std::string path = "1.csv";
	std::vector< std::vector<std::wstring> > woutlines;
	for(int i = 0; i < 5; i++)
	{
		std::vector<std::wstring> woutline;
		for(int j = 0; j < 3; j++)
		{
			woutline.push_back(_T("哈哈"));
		}
		woutlines.push_back(woutline);
	}
	generator(path, woutlines);
	std::vector< std::vector<std::string> > inlines;
	parse(path, inlines);
}