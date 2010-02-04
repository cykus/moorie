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

#ifndef __VOLNY_H
#define __VOLNY_H

#include "../Mailbox.h"
#include <string>

//! Mailbox implementation for volny.cz
class VolnyMailbox: public CMailBox
{
	private:
		std::string auth;
		int totalEmails;
		std::string page;

		std::string postlink;
		std::string base;
		std::string url;
		std::string myvars;
	public:
		VolnyMailbox(const std::string &name, const std::string &usr, const std::string &passwd);
		~VolnyMailbox();
		int loginRequest();
		void logoutRequest();
		void getHeadersRequest();
		int downloadRequest(int seg);
		int uploadRequest(std::string filename, std::vector<std::string> to, int seg);
		void parseResponse();
};
 #endif
