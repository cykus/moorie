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

#ifndef __GMAIL_H
#define __GMAIL_H

#include "../Mailbox.h"
#include <string>

//! Mailbox implementation for gmail.com
class GMailMailbox: public CMailBox
{
	private:
		std::string auth;
		int totalEmails;
		std::string page;

	public:
		GMailMailbox(const std::string &usr, const std::string &passwd);
		~GMailMailbox();
		int loginRequest();
		void logoutRequest();
		void getHeadersRequest();
		int downloadRequest(int seg);

		void parseResponse();
};

#endif

