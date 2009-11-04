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

#ifndef __MAILRU_H
#define __MAILRU_H

#include <string>
#include "../Mailbox.h"

class MailRuMailbox: public CMailBox
{
		std::string auth, page;
		int totalEmails;
		int pgcnt;


	public:
		MailRuMailbox(const string &usr, const string &passwd);
		~MailRuMailbox();
		int Login();
		void logoutRequest();
		int getHeadersRequest();
		void downloadRequest(/*const EmailHeader &hdr, Segment *s*/);

		void parseResponse();
};

#endif

