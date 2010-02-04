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

#include "Onet.h"
#include "../Log.h"
#include <sstream>
#include <boost/regex.hpp>
#include "../MailboxFactory.h"

namespace {
    CMailBox* Create(const std::string& name,
                     const std::string& username,
                     const std::string& password)
  {
    return new OnetMailbox(name, username, password);
  }

  const bool registered = MailboxFactory::Instance().
                                          Register("poczta.onet.pl", Create);


}

OnetMailbox::OnetMailbox(const std::string &name, const std::string &usr, const std::string &passwd)
                         : CMailBox(name, usr, passwd),
                           totalEmails( 0 )
{
}

int OnetMailbox::loginRequest()
{
	std::string page;
	std::string domain("poczta.onet.pl");
	page=doGet("http://lajt.onet.pl/poczta");
	const std::string vars = std::string("login=")
                + escape(getUser()) + escape("@"+domain) + "&password="
                + escape(getPassword()) + "&app_id=1&submit=OK";
	page=doPost("http://lajt.onet.pl/secure/index.html", vars,true);
	page=doGet("http://lajt.onet.pl/poczta");
	if (page.find(">Wyloguj <span ")>20)
	{
		auth = "http://lajt.onet.pl/poczta";
		return 0;
	}
	else
	{
		return 1;
	}
}

void OnetMailbox::logoutRequest()
{
	//setState(Mailbox::Disconnected); //TODO
}

void OnetMailbox::getHeadersRequest()
{
	std::string page;
	std::string url("http://lajt.onet.pl/folder_l.html?k=0&f=0");
	page = doGet(url);
	while (1>0)
	{
		boost::smatch match;
		boost::regex re("<a href=\"czytaj_l.html(.*?)\">(?:<b>)*(.+?)(?:<)(.*?)</a>");
		std::string link;
		std::string::const_iterator pbegin = page.begin();
		std::string::const_iterator pend = page.end();
		int msgcnt;
		while (boost::regex_search(pbegin,pend, match, re,boost::match_default))
		{
			link=match[1];
			boost::erase_all(link,"amp;");
			EmailHeader hdr(link, match[2]);
			//LOG(Log::Debug, "Found Header: "+hdr.subject);
			addHeader(hdr);
			addHeaderLink(link);
			pbegin = match[2].second;
			++msgcnt;
		}
		boost::smatch match2;
		boost::regex re2("([0-9]{1,2}),folder_l\\.html\\?k=0&amp;f=0\" class=\"link\">NastÄ™pne listy");
		boost::regex re3("([0-9]{1,2}),folder_l\\.html\\?k=0&amp;f=0\" class=\"link\">Zobacz starsze");
		boost::smatch match3;
		if (boost::regex_search(page,match,re2) /*&& !regex_search(page,match,re3)*/)
		{
			std::string url = "http://lajt.onet.pl/"+match[1]+",folder_l.html?k=0&f=0";
			page=doGet(url);
		}
		else if(boost::regex_search(page,match,re3))
		{
			std::string url = "http://lajt.onet.pl/"+match[1]+",folder_l.html?k=0&f=0";
			page=doGet(url);
		}
		else
		{
			break;
		}
	}
}

int OnetMailbox::downloadRequest(int seg/*const EmailHeader &hdr, Segment *s*/)
{
	std::string page;
	std::string mylink = getLink(seg);
	std::cout <<mylink<<std::endl;
	page=doGet("http://lajt.onet.pl/czytaj_l.html"+mylink);
	boost::regex re("<a href=\"zalacznik.html(.*?)\" class=\"link\">");
	boost::smatch match;
	std::string z;
	std::string::const_iterator pbegin = page.begin();
	std::string::const_iterator pend = page.end();
	while (boost::regex_search(pbegin,pend, match, re,boost::match_default))
	{
		z=match[1];
		boost::erase_all(z,"amp;");
		pbegin = match[2].second;
	}
	downloadSeg();
	doGet("http://lajt.onet.pl/zalacznik.html"+z);
	if (downloadSegDone() == 0)
		return 0;
	else
		return 1;
}

int OnetMailbox::uploadRequest(std::string filename, std::vector<std::string> to, int seg)
{
// 	const std::string vars = std::string("to=")
// 			+ escape(getUser()) + escape("@"+domain) + "&su="
// 			+ escape(getPassword()) + "&file0=" + filename + "&submit=Wy¶lij";

	std::string myvars;

	page = doGet("http://lajt.onet.pl/napisz_l.html?k=0");

	boost::regex re("<input type=\"hidden\" name=\"uid\" value=\"(.*?)\" /><input type");
	boost::smatch match;
	if (boost::regex_search(page,match,re) /*&& !regex_search(page,match,re3)*/)
	{
		myvars = match[1];
	}
	std::cout << page << std::endl;
	std::cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << myvars << std::endl;

	const std::string url = unescape("http://lajt.onet.pl/napisz_l.html");
	std::cout << " ------------------------------------------------------------------ " << std::endl;
//  	page = doHTTPUpload(url, myvars, filename, true);
	std::cout << " ------------------------------------------------------------------ " << std::endl;
// 	std::cout << page << std::endl;
	return 0;

}


OnetMailbox::~OnetMailbox()
{
}

void OnetMailbox::parseResponse()
{
	//const Mailbox::State state = getState();
}

