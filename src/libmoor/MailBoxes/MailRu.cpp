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

int MailRuMailbox::getHeadersRequest()
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

	regex re3("<td width=10% class=\"rovn tdl\"><img src=http://img.mail.ru/0.gif width=24 height=1><br>(.*?)</td>");
	smatch match3;
	regex_search(page,match3,re3);
	istringstream pg(match3[1]);
	int pages;
	pg >> pages; 
	pages = pages/25 + 1;
	cout << "match: " << match3[1] << " stron: " << pages;
	
	while (pgcnt < pages) {
		pgcnt++;
		numstr.str("");
		numstr << pgcnt;
		string url = "http://win.mail.ru/cgi-bin/msglist?folder=0&page="+numstr.str();
		page = doGet(url);

		regex mheadre("<td class=letavtor title=.*?</a></td>.*?<td class=lettem><a href=\"readmsg([^\"]*)\"[^<>]*>([^<>]*)</a></td>");
	//	const string page = getPage();
		string::const_iterator pbegin = page.begin();
		string::const_iterator pend = page.end();
		while (regex_search(pbegin, pend, match, mheadre, match_default)) {
			EmailHeader hdr(match[1], match[2]);
//			cout << match[1] << " " << match[2] << endl;
//			LOG(Log::Debug, "Found Header: "+hdr.subject);
			cout << "Add header: " << hdr.subject << " Link: " << match[1] << endl;
			addHeader(hdr);
//			addHeaderSubject(hdr.subject);
			addHeaderLink(match[1]);
			pbegin = match[2].second;
			++msgcnt;
		}
		cout << "." << endl;		
	}
	
	/*
	do {
		pgcnt++;
		numstr.str("");
		numstr << pgcnt;
		string url = "http://win.mail.ru/cgi-bin/msglist?folder=0&page="+numstr.str();
		page = doGet(url);

		regex mheadre("<td class=letavtor title=.*?</a></td>.*?<td class=lettem><a href=\"readmsg([^\"]*)\"[^<>]*>([^<>]*)</a></td>");
	//	const string page = getPage();
		string::const_iterator pbegin = page.begin();
		string::const_iterator pend = page.end();
		while (regex_search(pbegin, pend, match, mheadre, match_default)) {
			EmailHeader hdr(match[1], match[2]);
//			LOG(Log::Debug, "Found Header: "+hdr.subject);
			cout << "Add header: " << hdr.subject << endl;
			addHeader(hdr);
			pbegin = match[2].second;
			++msgcnt;
		}
		cout << "." << endl;
	} while (regex_search(page,match,re2));
	*/
	return msgcnt;
//	setState(Mailbox::ReadHeadersDone); */
}

int MailRuMailbox::downloadRequest(int seg)
{
	string mylink = getLink(seg);
//	cout << mylink << endl;
 	setCookie(auth);
 	page = doGet("http://win.mail.ru/cgi-bin/readmsg"+mylink);
//	regex re("http://a[a-z]*[0-9].mail.ru/cgi-bin/readmsg/.*?&mode=attachment&channel=");
 	regex re("<a href=\"(http://a[^\"]*/cgi-bin/readmsg/[^\"]*&mode=attachment&channel=)");
	
	smatch match;
	string link;
	if(regex_search(page,match,re))
	{
		link=match[1];
		cout << link << endl;
		downloadSeg();
		doGet(link);
		downloadSegDone();
//		setState(Mailbox::DownloadIP);
//		setSegment(s);
	}

}

MailRuMailbox::~MailRuMailbox()
{
}

void MailRuMailbox::parseResponse()
{
// 	const Mailbox::State state = getState();
}
