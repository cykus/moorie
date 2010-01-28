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

#include "OiComBr.h"
#include "../Log.h"
#include "../MailboxFactory.h"

// #include "Downloader.h"
// #include "MoorieException.h"
#include <sstream>
#include <boost/regex.hpp>

namespace {
        CMailBox* Create(const std::string& name,
                         const std::string& username,
					 const std::string& password)
	{
                return new OiComBrMailbox(name, username, password);
	}

	const bool registered = MailboxFactory::Instance().
			Register("oi.com.br", Create);
}

OiComBrMailbox::OiComBrMailbox(const std::string &name, const std::string &usr, const std::string &passwd)
    : CMailBox(name, usr, passwd),
      totalEmails( 0 )
{
}

int OiComBrMailbox::loginRequest()
{
	LOG(Log::Debug,"OiComBrMailbox::loginRequest");
// 	LOG(Log::Debug,"OiComBrMailbox::State");
	std::string page;
	const std::string vars = std::string("imapuser=")
		+ escape(getUser())
		+"&pass="
		+ escape(getPassword())
		+"&loginButton=Entrar";

// 	setState(Mailbox::LoginIP);
	page=doPost("http://webmail.oi.com.br/mail/redirect.php",vars);

        boost::regex re("http://webmail.oi.com.br/mail/redirect.php");
        boost::smatch match;

	if(regex_search(page,match,re))
	{
// 		setState(Mailbox::LoginError); //login failure
		return 1;
// 		throw MoorieException("Login failed");
	}
	else  {
// 	setState(Mailbox::LoginDone);
		return 0;
	}
// 	setState(Mailbox::Connected);


/*	smatch match;

	regex re("Set-Cookie: (Horde=.*?;)");
	if (regex_search(page, match, re)) {
		auth = match[1];
		LOG(Log::Debug,"OK");
		return 0;
	}
	else {
		LOG(Log::Debug,":(");
		return 0;
	} */
}

void OiComBrMailbox::logoutRequest()
{
	doGet("http://webmail.oi.com.br/mail/login.php?logout_reason=logout");
//	setState(Mailbox::Disconnected); //TODO
}

void OiComBrMailbox::getHeadersRequest()
{
	LOG_ENTER("OiComBrMailbox::getHeadersRequest");
	std::string page;
// 	setState(Mailbox::ReadHeadersIP); // request headers
	page=doGet("http://webmail.oi.com.br/mail/mailbox.php?mailbox=INBOX");
        boost::regex re("<b>Mensagens:</b>[^\\(\\)]*\\(([0-9]*) total");
        boost::smatch match;
        boost::regex_search(page,match,re);
	std::stringstream myint;
	std::stringstream(match[1]) >> totalEmails;
	for(curEmail=1;(curEmail-1)*20<totalEmails;curEmail++)
	{
		myint.str("");
		myint << curEmail;
		page=doGet("http://webmail.oi.com.br/mail/mailbox.php?page="+myint.str());
                re=boost::regex("<a href=\"([^\"]*)\" title=\"([^\"]*)\" id=\"subject");
		std::string::const_iterator pbegin = page.begin();
		std::string::const_iterator pend = page.end();
                while (boost::regex_search(pbegin, pend, match, re, boost::match_default))
		{
			EmailHeader hdr(match[1], match[2]);
// 			LOG(Log::Debug,"Subject: "+match[2]);
// 			LOG(Log::Debug, "Found header: " + hdr.subject);
			addHeader(hdr);
			addHeaderLink(match[1]);
			pbegin = match[2].second;
		}

	}
// 	setState(Mailbox::ReadHeadersDone);
}

int OiComBrMailbox::downloadRequest(int seg)
{
	std::string mylink = getLink(seg);
        std::cout << mylink << std::endl;
	setCookie(auth);
	std::string page = doGet("http://webmail.oi.com.br"+mylink);
//	regex re("http://a[a-z]*[0-9].mail.ru/cgi-bin/readmsg/.*?&mode=attachment&channel=");
        boost::regex re("<a href=\"(/services/download/[^\"]*)");

        boost::smatch match;
        std::string link;
	if(regex_search(page,match,re))
	{
		link=match[1];
		LOG(Log::Debug, link);
                std::string dlink = "http://webmail.oi.com.br"+link;
                boost::erase_all(dlink,"amp;");
		downloadSeg();
		doGet(dlink);
		if (downloadSegDone() == 0)
			return 0;
		else
			return 1;
//		setState(Mailbox::DownloadIP);
//		setSegment(s);
	}
}

int OiComBrMailbox::uploadRequest(std::string filename, std::string to, int seg)
{
}


OiComBrMailbox::~OiComBrMailbox()
{
}

void OiComBrMailbox::parseResponse()
{
// 	const Mailbox::State state = getState();
}
