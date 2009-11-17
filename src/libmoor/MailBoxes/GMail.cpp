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

#include "GMail.h"
#include "../Log.h"
#include "MailboxFactory.h"

#include <sstream>
#include <boost/regex.hpp>
#include <boost/format.hpp>

namespace {
  CMailBox* Create(const std::string& username, 
                   const std::string& password) 
  {
    return new GMailMailbox(username, password);
  }
  
  const bool registered = MailboxFactory::Instance().
                                          Register("gmail.com", Create);
}

GMailMailbox::GMailMailbox(const string &usr, const string &passwd)
  : CMailBox(usr, passwd), 
    totalEmails(0)
{
}

int GMailMailbox::loginRequest()
{
//	LOG_ENTER("GMailMailbox::loginRequest");
	smatch match2;
        page = doGet("https://www.google.com/accounts/ServiceLoginAuth");
        regex re3("name=\"GALX\"[\n].*?value=\"([a-zA-Z0-9_*[-]*]*)\"");
        regex_search(page, match2, re3);
        const string vars = string("&continue=")
		+ escape("http://mail.google.com/mail/?")
                + "&service=mail&rm=false&GALX="
                + escape(match2[1]) + "&Email="
		+ escape(getUser()) + "&Passwd="
		+ escape(getPassword())
                + "&signIn=Sign+in";
	
	page = doPost("https://www.google.com/accounts/ServiceLoginAuth", vars);

	regex re("url=&#39;(.+)&");
	regex re2("&amp;");
	regex authre("auth=([\\w\\d_-]+)");
	smatch match;
	if (regex_search(page, match, re))
	{
		string url = match[1];
		regex_replace(url, re2, "&");
		url = unescape(url);
		if (regex_search(url, match, authre))
			auth = match[1];
//		LOG(Log::Debug, "auth=" + auth);
		return 0;
	}
	else
	{
		return 1;
	}
}

void GMailMailbox::logoutRequest()
{
//	setState(Mailbox::Disconnected); //TODO
}

void GMailMailbox::getHeadersRequest()
{
//	LOG_ENTER("GMailMailbox::getHeadersRequest");
	string url("https://mail.google.com/mail/?ui=html&auth=");

	totalEmails = 0;
	page = doGet(url + auth);

	int msgcnt = 0; // number of message headers for current page

	match_results<string::const_iterator> match;
	regex mheadre("type=\"checkbox\".+?value=\"(.+?)\".+?<a.+?(?:</font>)+\\s*(?:<b>)*(.+?)(?:<)");

	while(regex_search(page,match,mheadre)) {
	//	const string page = getPage();
		string::const_iterator pbegin = page.begin();
		string::const_iterator pend = page.end();
		msgcnt = 0;
		while (regex_search(pbegin, pend, match, mheadre, match_default))
		{
			EmailHeader hdr(match[1], match[2]);
			addHeader(hdr);
			pbegin = match[2].second;
			++msgcnt;
		}
		if (msgcnt == 0)
		{
			
		}
		else
		{
			stringstream numstr;
			totalEmails += msgcnt;
			numstr << totalEmails;
			string url = "https://mail.google.com/mail/?ui=html&st=" + numstr.str();
			page = doGet(url);
		}
	}
	
}

int GMailMailbox::downloadRequest(int seg)
{
	string mylink = getLink(seg);
//	page = doGet();
	std::string link = "https://mail.google.com/mail/?realattid=file0&attid=0.1&disp=attd&view=att&th=" + mylink;
//	LOG(Log::Debug, link);
	downloadSeg();
	doGet(link);
	downloadSegDone();
}

GMailMailbox::~GMailMailbox()
{
}

void GMailMailbox::parseResponse()
{
//	const Mailbox::State state = getState();
}

