/*
 * This file is a part of Moorie
 *
 * Copyright (C) 2007-2010 Moorie Team - http://moorie.pl/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
  
#ifndef __ROCKCOM_H
#define __ROCKCOM_H

#include <string>
#include "../Mailbox.h"


//! Mailbox implementation for gmail.com
class RockComMailbox: public CMailBox
{
	private:
		std::string auth;
		int totalEmails;
		int pgcnt;
		std::string page;

	public:
                RockComMailbox(const std::string &name, const std::string &usr, const std::string &passwd);
		~RockComMailbox();
		int loginRequest();
		void logoutRequest();
		void getHeadersRequest();
		int downloadRequest(int seg);
		int uploadRequest(std::string filename, std::vector<std::string> to, int seg);

		void parseResponse();
};

#endif

