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
#include "configer.hpp"
#include "logger.hpp"

void configer::tester()
{
	configer conf("E:\\Work\\SMPPServer\\smppserver\\output\\configer.ini");
	std::string proxy = conf.get_string("sip", "default_proxy", "");
	conf.set_string("sip", "default_proxy", "sip.evoice.com");
	proxy = conf.get_string("sip", "default_proxy", "");
	double down = conf.get_double("net", "download_bw", 0);
	int up = conf.get_int("net", "upload_bw", 0);
	conf.set_double("net", "download_bw", 9999.9);
	conf.set_int("net", "upload_bw", 8888);
	down = conf.get_double("net", "download_bw", 0);
	up = conf.get_int("net", "upload_bw", 0);
	conf.sync();
	
	configer conf2("E:\\Work\\SMPPServer\\smppserver\\output\\configer.ini");
	proxy = conf2.get_string("sip", "default_proxy", "");
	down = conf2.get_double("net", "download_bw", 0);
	up = conf2.get_int("net", "upload_bw", 0);
	(void)down;
	(void)up;
}




