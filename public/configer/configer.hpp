/***************************************************************************
 *            configer.h
 *
 *  April 21 22:16:00 2013
 *  Copyright  2013  Zhu Nengjie, thanks  Linphone projects
 *  mail: Bifang technology
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#ifndef CONFIGER_H
#define CONFIGER_H
#include "algorithm.hpp"
#include <string>
#include <list>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/thread/mutex.hpp>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include "logger.hpp"

/**
 * The class Configer is used to manipulate a configuration file,  The format of the configuration file 
 * is a .ini like format:
 * - sections are defined in []
 * - each section contains a sequence of key=value pairs.
 *
 * Example:
 * @code
 * [smpp_timer]
 * inactivity_duration=30
 * response_duration=5
 *
 * [restrict_ip]
 * restrict=1
 * @endcode
**/


class configer_item   
{
private:
	std::string key_;
	std::string value_;
	boost::mutex mutex_;
public:
	configer_item(const std::string& key, const std::string& value)
	{
		key_ = key;
		value_ = value;
	}
	~configer_item()
	{
	}
	configer_item& operator=(const configer_item& other)
	{
		if (this != &other)  
		{  
			key_ = other.key_;
			value_ = other.value_;
		}  
		return *this; 
	}

	bool operator==(const configer_item &o)      
	{  
		return((key_.compare(o.key_) == 0) && (value_.compare(o.value_) == 0));
	}  

	bool is_this_item(const std::string& key)
	{
		return (key_.compare(key) == 0);
	}
	void write(const std::string& filename)
	{
#ifdef WIN32
		std::wstring wfilename = s2ws(filename);
		std::ofstream ofs(wfilename.c_str(), std::ios_base::app);
#else
		std::ofstream ofs(filename.c_str(), std::ios_base::app);
#endif
		boost::mutex::scoped_lock lock(mutex_);
		std::stringstream stream;
		stream<<key_<<"="<<value_<<"\n";
		std::string s = stream.str();
		ofs.write(s.c_str(), (std::streamsize)s.length());
		ofs.close();
	}
	std::string key(){return key_;}
	std::string value(){return value_;}

	void set_key(const std::string& key){key_ = key;}
	void set_value(const std::string& value){value_ = value;}
};
typedef boost::shared_ptr<configer_item> configer_item_ptr;

class configer_section
{
private:
	std::string name_;
	std::list<configer_item_ptr> items_;
	boost::mutex mutex_;
protected:
	void destroy()
	{
		boost::mutex::scoped_lock lock(mutex_);
		while(true)
		{
			items_.pop_back();
			if(items_.size() == 0)
			{
				break;
			}
		}
	}
public:
	configer_section(const std::string& name)
	{
		name_ = name;
	}
	~configer_section()
	{
		destroy();
	}
	void add_item(const configer_item_ptr& item_ptr)
	{
		boost::mutex::scoped_lock lock(mutex_);
		items_.push_back(item_ptr);
	}

	bool is_this_section(const std::string& name)
	{
		return (name_.compare(name) == 0);
	}

	const boost::optional<configer_item_ptr> find_item(const std::string& key)
	{
		boost::mutex::scoped_lock lock(mutex_);
		boost::optional<configer_item_ptr> i(boost::none);
		std::list<configer_item_ptr>::iterator itor = items_.begin();
		while(itor != items_.end())
		{
			if((*itor)->is_this_item(key))
			{
				return (*itor);
			}
			itor++;
		}
		return i;
	}
	void write(const std::string& filename )
	{
#ifdef WIN32
		std::wstring wfilename = s2ws(filename);
		std::ofstream ofs(wfilename.c_str(), std::ios_base::app);
#else
		std::ofstream ofs(filename.c_str(), std::ios_base::app);
#endif
		boost::mutex::scoped_lock lock(mutex_);
		std::stringstream stream;
		std::string s;
		stream<<"["<<name_<<"]"<<"\n";
		s = stream.str();
		stream.clear();
		ofs.write(s.c_str(), s.length());
		
		std::list<configer_item_ptr>::iterator itor = items_.begin();
		while(itor != items_.end())
		{
			(*itor)->write(filename);
			itor++;
		}
		ofs.close();
	}
};
typedef boost::shared_ptr<configer_section> configer_section_ptr;

inline bool is_first_char(const char *start, const char *pos)
{
	const char *p;
	for(p = start; p < pos; p++)
	{
		if (*p != ' ') return false;
	}
	return true;
}

class configer
{
protected:
	std::string filename_;
	std::list<configer_section_ptr> sections_;
	int modified_;
	bool readonly_;
	boost::mutex mutex_;
protected:
	bool parse()
	{
		char tmp[8192];
		boost::optional<configer_section_ptr> section;
#ifdef WIN32
		std::wstring wfilename = s2ws(filename_);
		std::ifstream ifs(wfilename.c_str());
#else
		std::ifstream ifs(filename_.c_str());
#endif
		if(!ifs.is_open())
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "open configer file %s failed. ", filename_.c_str());
			return false;
		}

		while(ifs.getline(tmp, 8192))
		{
			char *pos1,*pos2;
			pos1=strchr(tmp,'[');
			if (pos1!=NULL && is_first_char(tmp, pos1) )
			{
				pos2=strchr(pos1,']');
				if (pos2!=NULL)
				{
					int nbs;
					char secname[256] = {0};
					/* found section */
					*pos2='\0';
					nbs = sscanf(pos1+1, "%s", secname);
					if (nbs == 1 )
					{
						if (strlen(secname)>0)
						{
							section = find_section(secname);
							if (!section)
							{
								configer_section_ptr section_ptr(new configer_section(secname)); 
								section.reset(section_ptr);
								sections_.push_back(section_ptr);
							}
						}
					}
					else
					{
						logger_log(__FUNCTION__, LEVEL_WARNING, "parse %s failed", tmp);
					}
				}
			}
			else
			{
				pos1=strchr(tmp,'=');
				if (pos1!=NULL)
				{
					char key[256] = {0};
					*pos1 = '\0';
					if (sscanf(tmp,"%s",key)>0)
					{
						pos1++;
						pos2 = strchr(pos1,'\r');
						if ( pos2 == NULL)
						{
							pos2 = strchr(pos1,'\n');
						}
						if (pos2 == NULL)
						{
							pos2 = pos1 + strlen(pos1);
						}
						else 
						{
							*pos2 = '\0'; /*replace the '\n' */
							pos2--;
						}
						/* remove ending white spaces */
						for (; pos2 > pos1 && *pos2 == ' ';pos2--) *pos2 = '\0';
						if (pos2 - pos1 >= 0)
						{
							/* found a pair key,value */
							if (section)
							{
								const boost::optional<configer_item_ptr> item = section.get()->find_item(key);
								if (!item)
								{
									configer_item_ptr item_1(new configer_item(key, pos1));
									section.get()->add_item(item_1);
								}
								else
								{
									item.get()->set_value(pos1);
								}
								
							}
							else
							{
								logger_log(__FUNCTION__, LEVEL_WARNING, "parse %s failed", tmp);
							}
						}
					}
				}
			}
		}
		ifs.close();
		return true;
	}
	void destroy()
	{
		boost::mutex::scoped_lock lock(mutex_);
		while(true)
		{
			if(sections_.size() == 0)
			{
				break;
			}
			sections_.pop_back();
		}
	}
const boost::optional<configer_section_ptr> find_section(const std::string& name)
{
	boost::optional<configer_section_ptr> s(boost::none);
	std::list<configer_section_ptr>::iterator itor = sections_.begin();
	while(itor != sections_.end())
	{
		if((*itor)->is_this_section(name))
		{
			return *itor;
		}
		itor++;
	}
	return s;
}
	const boost::optional<configer_item_ptr> find_item(const std::string& name, const std::string& key);
public:
	configer(const std::string& filename)
	{
		filename_ = filename;
		parse();
	}
	~configer()
	{
		destroy();
	}

public:
	 //Retrieves a configuration item as a string, given its section, key, and default value.
	 //The default value string is returned if the config item isn't found.
	std::string get_string(const std::string& name, const std::string& key, const std::string& default_string)
	{
		boost::mutex::scoped_lock lock(mutex_);
		boost::optional<configer_item_ptr> item;
		boost::optional<configer_section_ptr> section =find_section(name);
		if (section)
		{
			item = section.get()->find_item(key);
			if (item)
			{
				return item.get()->value();
			}
			else
			{
				//logger_log(__FUNCTION__, LEVEL_WARNING, "section[%s]-item(%s) of %s not exist.", name.c_str(),
				//	key.c_str(), filename_.c_str());
				return default_string;
			}
		}
		else
		{
			logger_log(__FUNCTION__, LEVEL_WARNING, "section[%s] of %s not exist.", name.c_str(), filename_.c_str());
			return default_string;
		}
	}

	//Retrieves a configuration item as an integer, given its section, key, and default value.
	// The default integer value is returned if the config item isn't found.
	int get_int(const std::string& name, const std::string& key, int default_value)
	{
		std::string value = get_string(name, key, "");
		if(value.length() > 0)
		{
			std::stringstream stream;
			int v;
			stream<<value;
			stream>>v;
			return v;
		}
		return default_value;
	}

	//Retrieves a configuration item as a 64 bit integer, given its section, key, and default value.
	// The default integer value is returned if the config item isn't found.
	boost::int64_t get_int64(const std::string& name, const std::string& key, boost::int64_t default_value)
	{
		std::string value = get_string(name, key, "");
		if(value.length() > 0)
		{
			int v;
			std::stringstream stream;
			stream<<value;
			stream>>v;
			return v;
		}
		return default_value;
	}

	// Retrieves a configuration item as a double, given its section, key, and default value.
	//The default double value is returned if the config item isn't found.
	double get_double(const std::string& name, const std::string& key, double default_value)
	{
		std::string value = get_string(name, key, "");
		if(value.length() > 0)
		{
			double v;
			std::stringstream stream;
			stream<<value;
			stream>>v;
			return v;
		}
		return default_value;
	}
	//Sets a string config item 
	void set_string(const std::string& name, const std::string& key,  const std::string& value)
	{
		boost::mutex::scoped_lock lock(mutex_);
		boost::optional<configer_item_ptr> item;
		boost::optional<configer_section_ptr> section =find_section(name);
		if (section)
		{
			item = section.get()->find_item(key);
			if (item)
			{
				item.get()->set_value(value);
			}
			else
			{
				configer_item_ptr item_ptr(new configer_item(key,value));
				section.get()->add_item(item_ptr);
			}
		}
		else
		{
			configer_section_ptr section_ptr(new configer_section(name));
			sections_.push_back(section_ptr);

			configer_item_ptr item_ptr(new configer_item(key,value));
			section_ptr.get()->add_item(item_ptr);
		}
		modified_++;
	}
	//Sets an integer config item
	void set_int(const std::string& section, const std::string& key,  int value)
	{
		std::stringstream stream;
		stream<<value;
		std::string s = stream.str();
		set_string(section, key, s);
	}
	// Sets a 64 bits integer config item
	void set_int64(const std::string& section, const std::string& key, boost::int64_t value)
	{
		std::stringstream stream;
		stream<<value;
		std::string s = stream.str();
		set_string(section, key, s);
	}
	//Sets a double config item
	void set_double(const std::string& section, const std::string& key, double value)
	{
		std::stringstream stream;
		stream<<value;
		std::string s = stream.str();
		set_string(section, key, s);
	}

	bool need_commit()
	{
		return (modified_ > 0);
	}
	bool section_exists(const std::string& section)
	{
		boost::mutex::scoped_lock lock(mutex_);
		return false;
	}
	void add_section(const configer_section_ptr& section_ptr)
	{
		boost::mutex::scoped_lock lock(mutex_);
		sections_.push_back(section_ptr);
	}
	int sync()
	{
		boost::mutex::scoped_lock lock(mutex_);
#ifdef WIN32
		std::wstring wfilename = s2ws(filename_);
		std::ofstream ofs(wfilename.c_str());
#else
		std::ofstream ofs(filename_.c_str());
#endif
		ofs.close();
		std::list<configer_section_ptr>::iterator itor = sections_.begin();
		while(itor != sections_.end())
		{
			(*itor)->write(filename_);
			itor++;
		}
		return 0;
	}

	static void tester();
};

#endif
