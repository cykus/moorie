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

#ifndef __YAHOO_H
#define __YAHOO_H

#include "../Mailbox.h"
#include <string>

using namespace std;

//! Mailbox implementation for yahoo.com
class YahooMailbox: public CMailBox
{
	private:
		string auth;
		string uvar;
		string challenge;
		string mserv;
		int totalEmails;
		int curEmail;
		int pgcnt;

	public:
		YahooMailbox(const std::string &usr, const std::string &passwd);
		~YahooMailbox();
		int loginRequest();
		void logoutRequest();
		void getHeadersRequest();
		int downloadRequest(int seg);

		void parseResponse();
};

#endif

