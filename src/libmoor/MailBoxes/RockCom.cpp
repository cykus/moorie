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

#include "RockCom.h"
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
                return new RockComMailbox(name, username, password);
	}

	const bool registered = MailboxFactory::Instance().
			Register("rock.com", Create);
}


RockComMailbox::RockComMailbox(const std::string &name, const std::string &usr, const std::string &passwd)
    : CMailBox(name, usr, passwd),
      totalEmails( 0 )
{
}

int RockComMailbox::loginRequest()
{
// 	LOG_ENTER("RockComMailbox::loginRequest");
	const std::string vars = std::string("show_frame=Enter&action=login&domain=")
		+ escape("rock.com")
		+ "&mail_language=us&longlogin=1&login="
		+ escape(getUser()) + "&password="
		+ escape(getPassword());
		//+ "&submit.x=41&submit.y=8";
// 	setState(Mailbox::LoginIP);
// 	LOG_ENTER(getUser());
// 	LOG_ENTER(getPassword());
	page=doPost("http://webmail.rock.com/scripts/common/proxy.main", vars);
	boost::regex re("a");
	boost::smatch match;
	if (boost::regex_search(page, match, re))
	{
// 		setState(Mailbox::LoginDone); // logged in
		auth = match[1];
// 		LOG(Log::Debug, "auth=" + auth);
		return 0;
	}
	else
	{
// 		setState(Mailbox::LoginError); //login failure
		return 1;
	}
}

void RockComMailbox::logoutRequest()
{
//	setState(Mailbox::Disconnected); //TODO
}

void RockComMailbox::getHeadersRequest()
{
// 	LOG_ENTER("RockComMailbox::getHeadersRequest");
	std::string url("http://mymail.rock.com/scripts/mail/mailbox.mail?folder=INBOX"),page;
// 	setState(Mailbox::ReadHeadersIP); // request headers
	totalEmails = 0;
	pgcnt=0;
	page=doGet(url + auth);
	while(1)
	{
		//cout << "Read Headers In Progress" << endl;
		boost::match_results<std::string::const_iterator> match;
//		regex mheadre("<a href=\"/scripts/mail/(.*?) onclick=.+?title=\"(?:<B> )(.*?)(?: </B>)\">");
		boost::regex mheadre("<a href=\"/scripts/mail/(.*?) onclick=.+?title=\"(?:<B>)*(.*?)(?: </B>)*\">");
		std::string::const_iterator pbegin = page.begin();
		std::string::const_iterator pend = page.end();
		while (boost::regex_search(pbegin, pend, match, mheadre, boost::match_default))
		{
			EmailHeader hdr(match[1], match[2]);
			//cout << match[2] << endl;
//			LOG(Log::Debug, "Found header: " + hdr.subject);
	//		cout << hdr.subject << endl;
			addHeader(hdr);
			addHeaderLink(match[1]);
			pbegin = match[2].second;
		}
        boost::regex re2("<a href=\"/scripts/[^<>]*\"><img src=\"http://img1.us4.outblaze.com/rock.com/nextPg.gif\"");
        boost::smatch match2;

		if (!boost::regex_search(page,match,re2))
		{
// 			setState(Mailbox::ReadHeadersDone);
			break;
		}
		else
		{
			//cout << match[2] << endl;
			std::stringstream numstr;
			pgcnt++;
			numstr << pgcnt*50+1;
			std::string url = "http://mymail.rock.com/scripts/mail/mailbox.mail?folder=INBOX&order=Newest&.ob=2013874a6a87e3165de301dc859da6acf78d4520&mview=a&mstart="+numstr.str()+";";
			page=doGet(url);
		}
	}
}

int RockComMailbox::downloadRequest(int seg)
{
// 	LOG_ENTER("RockComMailbox::downloadRequest");
	//cout << hdr.uniqueId << endl;
// 	string page=doGet("http://mymail.rock.com/scripts/mail/"+hdr.uniqueId);
// 	smatch match;
// 	regex re("<A HREF=\"(/getattach/[^\"]*)");
// 	if(!regex_search(page,match,re))
// 		return 1;
// 	else
// 		return 0;
// 		throw MoorieException("Unable to download attachement.");
// 	setState(Mailbox::DownloadIP);
// 	setSegment(s);
// 	doGet("http://mymail.rock.com"+match[1]);
	std::string mylink = getLink(seg);
	setCookie(auth);
	page = doGet("http://mymail.rock.com/scripts/mail/"+mylink);
	boost::regex re("<A HREF=\"(/getattach/[^\"]*)");

	boost::smatch match;
	std::string link, downlink;
 	if(boost::regex_search(page,match,re)) {
		link=match[1];
//		cout << link << endl;
		downlink = "http://mymail.rock.com"+link;
		LOG(Log::Debug, downlink);
		downloadSeg();
		doGet(downlink);
		if (downloadSegDone() == 0)
			return 0;
		else
			return 1;
//		setState(Mailbox::DownloadIP);
//		setSegment(s);
	} else
		return 1;

}

int RockComMailbox::uploadRequest(std::string filename, std::vector<std::string> to, int seg)
{
}


RockComMailbox::~RockComMailbox()
{
}

void RockComMailbox::parseResponse()
{
}

