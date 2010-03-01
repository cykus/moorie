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
        
#ifndef __OICOMBR_H
#define __OICOMBR_H

#include "../Mailbox.h"
#include <string>

//! Mailbox implementation for yahoo.com
class OiComBrMailbox: public CMailBox
{
	private:
                std::string auth;
                std::string uvar;
                std::string challenge;
                std::string mserv;
		int totalEmails;
		int curEmail;
		int pgcnt;

	public:
                OiComBrMailbox(const std::string &name, const std::string &usr, const std::string &passwd);
		~OiComBrMailbox();
		int loginRequest();
		void logoutRequest();
		void getHeadersRequest();
		int downloadRequest(int seg);
		int uploadRequest(std::string filename, std::vector<std::string> to, int seg);

		void parseResponse();
};

#endif

