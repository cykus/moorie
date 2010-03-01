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

#ifndef __GOOGLEAPPS_H
#define __GOOGLEAPPS_H

#include "../Mailbox.h"
#include <string>

class GoogleAppsMailbox: public CMailBox
{
    public:
	private:
                std::string auth;
                int totalEmails;
                std::string page;

				std::string postlink;
				std::string base;
				std::string url;
				std::string myvars;
	public:
                GoogleAppsMailbox(const std::string &name, const std::string &usr, const std::string &passwd);
		~GoogleAppsMailbox();
                int loginRequest();
                void logoutRequest();
                void getHeadersRequest();
                int downloadRequest(int seg);
				int uploadRequest(std::string filename, std::vector<std::string> to, int seg);

                void parseResponse();
};
#endif

