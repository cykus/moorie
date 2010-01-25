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
#include "../MailboxFactory.h"

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

GMailMailbox::GMailMailbox(const std::string &usr, const std::string &passwd)
  : CMailBox(usr, passwd),
    totalEmails(0)
{
}

int GMailMailbox::loginRequest()
{
//	LOG_ENTER("GMailMailbox::loginRequest");
	boost::smatch match2;
        page = doGet("https://www.google.com/accounts/ServiceLoginAuth");
        boost::regex re3("name=\"GALX\"[\n].*?value=\"([a-zA-Z0-9_*[-]*]*)\"");
        boost::regex_search(page, match2, re3);
        const std::string vars = std::string("&continue=")
		+ escape("http://mail.google.com/mail/?")
                + "&service=mail&rm=false&GALX="
                + escape(match2[1]) + "&Email="
		+ escape(getUser()) + "&Passwd="
		+ escape(getPassword())
                + "&signIn=Sign+in";

	page = doPost("https://www.google.com/accounts/ServiceLoginAuth", vars);

	boost::regex re("url=&#39;(.+)&");
	boost::regex re2("&amp;");
	boost::regex authre("auth=([\\w\\d_-]+)");
	boost::smatch match;
	if (boost::regex_search(page, match, re))
	{
                std::string url = match[1];
                if(url.find("answer=40695") != std::string::npos)
                {
                    LOG(Log::Info, "Niestety, konto zostało wyłączone.");
                    return 1;
                }
		boost::regex_replace(url, re2, "&");
		url = unescape(url);
		if (boost::regex_search(url, match, authre))
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
	std::string url("https://mail.google.com/mail/?ui=html&auth=");

	totalEmails = 0;
	page = doGet(url + auth);

	int msgcnt = 0; // number of message headers for current page

	boost::match_results<std::string::const_iterator> match;
	boost::regex mheadre("type=\"checkbox\".+?value=\"(.+?)\".+?<a.+?(?:</font>)+\\s*(?:<b>)*(.+?)(?:<)");

	while(boost::regex_search(page,match,mheadre)) {
	//	const string page = getPage();
		std::string::const_iterator pbegin = page.begin();
		std::string::const_iterator pend = page.end();
		msgcnt = 0;
		while (boost::regex_search(pbegin, pend, match, mheadre, boost::match_default))
		{
			EmailHeader hdr(match[1], match[2]);
			addHeader(hdr);
			addHeaderLink(match[1]);
			pbegin = match[2].second;
			++msgcnt;
		}
		if (msgcnt == 0)
		{

		}
		else
		{
			std::stringstream numstr;
			totalEmails += msgcnt;
			numstr << totalEmails;
			std::string url = "https://mail.google.com/mail/?ui=html&st=" + numstr.str();
			page = doGet(url);
		}
	}

}

int GMailMailbox::downloadRequest(int seg)
{
	std::string mylink = getLink(seg);
//	page = doGet();
	std::string link = "https://mail.google.com/mail/?realattid=file0&attid=0.1&disp=attd&view=att&th=" + mylink;
//	LOG(Log::Debug, link);
	downloadSeg();
	doGet(link);
	if (downloadSegDone() == 0) return 0;
	else return 1;
}

int GMailMailbox::uploadRequest(std::string filename, std::string to, int seg) {
	std::string segCRC = getSegCRC(filename);

	url = "https://mail.google.com/mail/h/?v=b&pv=tl&cs=b";

	page = doGet(url);

	// wyszukiwanie base hrefa
	boost::regex re1(" <base href=\"(.*?)\"> ");
	boost::smatch match1;
	if (boost::regex_search(page, match1, re1)) {
		base = match1[1];
	} else
		return 1;

	// szukanie linka do POST-a
	boost::regex re("<td bgcolor=\"#e0ecff\"> <form action=\"(.*?)\" name=\"f\"");
	boost::smatch match;
	if (boost::regex_search(page, match, re)) {
		postlink = base + match[1];
	} else
		return 1;

	variables my_vars;
	my_vars.to_form = "to";
	my_vars.to_address = to;
	my_vars.subject_form = "subject";
	my_vars.subject = EncodeHeader(filename, segCRC, getFileCRC(), seg);
	my_vars.body_form = "body";
	my_vars.body = "tresc wiadomosci";
	my_vars.submit_form = "nvp_bu_send";
	my_vars.submit = "Wy�ij";
	page = doHTTPUpload(postlink, my_vars, filename, true);

	return 0;
}

GMailMailbox::~GMailMailbox()
{
}

void GMailMailbox::parseResponse()
{
//	const Mailbox::State state = getState();
}

