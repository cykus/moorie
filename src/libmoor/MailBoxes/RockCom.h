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
		int uploadRequest(std::string filename, std::string to, int seg);

		void parseResponse();
};

#endif

