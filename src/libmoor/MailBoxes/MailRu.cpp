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

#include "MailRu.h"
// #include "Log.h"
// #include "Downloader.h"
// #include "MoorieException.h"
#include <sstream>
#include <boost/regex.hpp>


MailRuMailbox::MailRuMailbox(const string &usr, const string &passwd): CMailBox(usr, passwd), totalEmails(0)

{
}

int MailRuMailbox::Login()
{
	
//	LOG_ENTER("MailRuMailbox::loginRequest");
	const string vars = string("Login=")
		+ escape(getUser()) + "&Domain=" + escape("mail.ru") + "&Password="
		+ escape(getPassword()) + "&level=1";
//	setState(Mailbox::LoginIP);
	page = doPost("http://win.mail.ru/cgi-bin/auth", vars,true);
	smatch match;
//	page = doGet("http://win.mail.ru/cgi-bin/msglist?folder=0");

	regex re("Set-Cookie: (Mpop=.*?;)");
	if (regex_search(page, match, re))
	{	
		auth = match[1];
//		LOG(Log::Debug, "auth=" + auth);
//		setState(Mailbox::LoginDone); // logged in
		return 0;
	}
	else
	{
//		setState(Mailbox::LoginError); //login failure
//		throw MoorieException("Login failed");
		return 1;
	}
//	setState(Mailbox::Connected);
}

void MailRuMailbox::logoutRequest()
{
	/*
	setState(Mailbox::Disconnected); //TODO
	*/
}

void MailRuMailbox::getHeadersRequest()
{
//	LOG_ENTER("MailRuMailbox::getHeadersRequest");
	string url("http://win.mail.ru/cgi-bin/msglist?folder=0");
	//setState(Mailbox::ReadHeadersIP); // request headers
// 	totalEmails = 0;
//     setCookie(auth);
 	page = doGet(url);

	int msgcnt = 0; // number of message headers for current page
	static int pgcnt = 0;
	stringstream numstr;
	match_results<string::const_iterator> match;

	regex re2("<a href=\".*?\" id=\"nextbut\">.*?\n&nbsp;&nbsp;<a");
	smatch match2;

	while (regex_search(page,match,re2))
	{
		pgcnt++;
		cout << pgcnt << endl;
		numstr.str("");
		numstr << pgcnt;
		string url = "http://win.mail.ru/cgi-bin/msglist?folder=0&page="+numstr.str();
		page = doGet(url);

		regex mheadre("<td class=letavtor title=.*?</a></td>.*?<td class=lettem><a href=\"readmsg([^\"]*)\"[^<>]*>([^<>]*)</a></td>");
	//	const string page = getPage();
		string::const_iterator pbegin = page.begin();
		string::const_iterator pend = page.end();
		while (regex_search(pbegin, pend, match, mheadre, match_default)) {
//			EmailHeader hdr(match[1], match[2]);
//			LOG(Log::Debug, "Found Header: "+hdr.subject);
//			addHeader(hdr);
			pbegin = match[2].second;
			++msgcnt;
			cout << msgcnt;
		}
	}
	cout << "Znaleziono w sumie " << msgcnt << " segmentow." << endl;

//	setState(Mailbox::ReadHeadersDone); */
}

void MailRuMailbox::downloadRequest()
{

}

MailRuMailbox::~MailRuMailbox()
{
}

void MailRuMailbox::parseResponse()
{
// 	const Mailbox::State state = getState();
}
