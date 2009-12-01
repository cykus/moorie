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
                std::string domain;

	public:
                GoogleAppsMailbox(const std::string &usr, const std::string &passwd);
		~GoogleAppsMailbox();
                int loginRequest();
                void logoutRequest();
                void getHeadersRequest();
                int downloadRequest(int seg);

                void parseResponse();
};
#endif

