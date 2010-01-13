/*
 * This file is a part of Moorie.
 *
 * Copyright (C) 2007 Krzysztof Konieczny <silent@aol.pl>
 *
 * Moorie is free software; you can redestribute it and/or modify it
 * under terms of GNU General Public License by Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY. See GPL for more details.
 */

#ifndef __ONET_H
#define __ONET_H

#include "../Mailbox.h"
#include <string>

class OnetMailbox: public CMailBox
{
	private:
		std::string auth,domain,page;
		int totalEmails;
		int pgcnt;
	public:
		OnetMailbox(const std::string &usr, const std::string &passwd);
		~OnetMailbox();
		int loginRequest();
		void logoutRequest();
		void getHeadersRequest();
		int downloadRequest(int seg);
		int uploadRequest(std::string filename);

		void parseResponse();
};

#endif

