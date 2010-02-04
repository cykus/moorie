/*
 * This file is a part of Moorie.
 *
 * Copyright (C) 2007 Pawel Stolowski <pawel.stolowski@wp.pl>
 *
 * Moorie is free software; you can redestribute it and/or modify it
 * under terms of GNU General Public License by Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY. See GPL for more details.
 */

#ifndef __MYNET_H
#define __MYNET_H

#include "../Mailbox.h"
#include <string>


class MyNetMailbox: public CMailBox
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
                MyNetMailbox(const std::string &name, const std::string &usr, const std::string &passwd);
		~MyNetMailbox();
		int loginRequest();
		void logoutRequest();
		void getHeadersRequest();
		int downloadRequest(int seg);
		int uploadRequest(std::string filename, std::vector<std::string> to, int seg);

		void parseResponse();
};

#endif

