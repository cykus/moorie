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

#include "Yahoo.h"
#include "../Log.h"
#include "../MailboxFactory.h"

//#include "Downloader.h"
//#include "MoorieException.h"
#include <sstream>
#include <boost/regex.hpp>
#include <boost/format.hpp>

namespace {
  CMailBox* Create(const std::string& username, 
                   const std::string& password) 
  {
    return new YahooMailbox(username, password);
  }
  
  const bool registered = MailboxFactory::Instance().
                                          Register("yahoo.com", Create);
}

YahooMailbox::YahooMailbox(const string &usr, const string &passwd): CMailBox(usr, passwd), totalEmails(0)
{
}

int YahooMailbox::loginRequest()
{
	//LOG(Log::Debug,"YahooMailbox::loginRequest");
	//LOG(Log::Debug,"YahooMailbox::State");
	string page;
	page=doGet("https://login.yahoo.com/config/mail?.intl=us");
	regex re("name=\".challenge\" value=\"([^\"]*)");
	smatch match;
	if (regex_search(page, match, re))
	{
		challenge=match[1];
		//LOG(Log::Debug, "challenge = " + challenge);
		regex re("name=\".u\" value=\"([^\"]*)");
		regex_search(page,match, re);
		uvar=match[1];
		//LOG(Log::Debug, ".u = " + uvar);
	}
	else
	{
		//LOG(Log::Debug,"Getting vars failes");
		//throw MoorieException("Unable to get login vars.");
		//return 1;
	}
	
	const string vars = string(".tries=1&.src=ym&.md5=&.hash=&.js=&.last=&promo=&.intl=us")
		+"&.bypass=&.partner=&.u="+uvar+"&.v=0&.challenge="+challenge+"&.yplus=&.emailCode=&pkg=&stepid="
		+"&.ev=&hasMsgr=0&.chkP=y&.done="
		+escape("http://mail.yahoo.com")
		+"&.pd="
		+escape("ym_ver=0&c=&ivt=&sg=")
		+"&login="
		+ escape(getUser())
		+"&passwd="
		+ escape(getPassword())
		+"&.persistent=y&.save=Sign in";
		//+ "&submit.x=41&submit.y=8";
	//setState(Mailbox::LoginIP);
	//LOG_ENTER(getUser());
	//LOG_ENTER(getPassword());
	page=doPost("https://login.yahoo.com/config/login?", vars);
	regex re3("window.location.replace.\"([^\"]*)");
	smatch match3,match4;
	string token,stoken,secdata;
	regex re4("(https://ab.login.yahoo.com/img/[^\"]*)");
	if(regex_search(page,match4,re4))
	{
		token=match4[1];
		regex re4("name=\".challenge\" value=\"([^\"]*)");
		regex_search(page,match4,re4);
		challenge=match4[1];
		re4=regex("name=\".u\" value=\"([^\"]*)");
		regex_search(page,match4,re4);
		uvar=match4[1];
		re4=regex("name=\".secdata\" value=\"([^\"]*)");
		regex_search(page,match4,re4);
		secdata=match4[1];
		LOG(Log::Debug, "challenge = " + challenge);
		LOG(Log::Debug, ".u = " + uvar);
		LOG(Log::Debug, ".secdata = " + secdata);
		LOG(Log::Debug, "token = " + token);
		cout << "Account blocked to unlock please enter token from this address:\n" << token << endl;
		cin >> stoken;
		//LOG(Log::Debug, "stoken = " + stoken);
		const string vars = string(".tries=1&.src=ym&.md5=&.hash=&.js=&.last=&promo=&.intl=us")
		+"&.bypass=&.partner=&.u="+uvar+"&.v=0&.challenge="+challenge+"&.yplus=&.emailCode=&pkg=&stepid="
		+"&.ev=&hasMsgr=0&.chkP=y&.done="
		+escape("http://mail.yahoo.com")
		+"&.pd="
		+escape("ym_ver=0&c=&ivt=&sg=")
		+"&login="
		+ escape(getUser())
		+"&passwd="
		+ escape(getPassword())
		+"&.secdata="+escape(secdata)
		+"&.secword="+escape(stoken)
		+"&.persistent=y&.save=Sign in";
		//LOG_ENTER(getUser());
		//LOG_ENTER(getPassword());
		doPost("https://login.yahoo.com/config/login?", vars);
	}
	else if (regex_search(page,match3, re3))
	{
		//setState(Mailbox::LoginDone); // logged in
		auth = match3[1];
		//LOG(Log::Debug, "auth=" + auth);
		page=doGet(auth);
	}
	else
	{
		//setState(Mailbox::LoginError); //login failure
		return 1;
	}
	string url;
	//LOG(Log::Debug,"auth: "+auth);
	mserv=auth.substr(0,auth.find("com")+3);
	url=mserv+"/ym/login?ymv=0";
	//LOG(Log::Debug,"Login Done");
	page=doGet(url);
	re=regex("Refresh CONTENT=\"0; URL='([^']*)");
	regex_search(page,match,re);
	auth=match[1];
	page=doGet(auth);
	//setState(Mailbox::Connected);
	mserv=auth.substr(0,auth.find("/mc/welcome?"));
	regex re2("<a href=\"(showFolder.fid=Inbox[^\"]*)");
	smatch match2;
	regex_search(page,match2,re2);
	auth=match2[1];
	auth=mserv+"/mc/"+auth;
	//LOG(Log::Debug, auth);
	return 0;
}

void YahooMailbox::logoutRequest()
{
	//setState(Mailbox::Disconnected); //TODO
}

void YahooMailbox::getHeadersRequest()
{
	//LOG_ENTER("YahooMailbox::getHeadersRequest");
	//setState(Mailbox::ReadHeadersIP); // request headers
	totalEmails = 0;
	smatch match2;
	string page=doGet(auth);
	stringstream myint;
	int msgcnt = 0; // number of message headers for current page
	match_results<string::const_iterator> match;
	regex how_many("<li id=\"inbox\" title=\"Inbox contains ([0-9]*) messages");
	regex how_many2("<span class=\"last\">Messages 1-25 of ([0-9]*)</span>"); 
	smatch hmatch;
	regex_search(page,hmatch,how_many2);
	std::cout << "ilosc: " <<hmatch[1] <<std::endl ;
	stringstream(hmatch[1]) >> totalEmails;
	regex mheadre("<a href=\"(showMessage[^\"]*)[^<>]*>([^<>]*)</a>");
	regex re2("<a href=\"(showFolder.fid=Inbox[^\"]*)");
	for(curEmail=25;msgcnt<=totalEmails;curEmail+=25)
	{
		string::const_iterator pbegin = page.begin();
		string::const_iterator pend = page.end();
		while (regex_search(pbegin, pend, match, mheadre, match_default))
		{
			EmailHeader hdr(match[1], match[2]);
			//LOG(Log::Debug, "Found header: " + hdr.subject);
			addHeader(hdr);
			addHeaderLink(match[1]);
			pbegin = match[2].second;
			++msgcnt;
		}
		regex_search(page,match2,re2);
		myint.str("");
		myint << curEmail;
		auth=mserv+"/mc/"+match2[1]+"&startMid="+myint.str();
		page=doGet(auth);
	} 
	//setState(Mailbox::ReadHeadersDone);
}

int YahooMailbox::downloadRequest(int seg)
{
	//LOG_ENTER("YahooMailbox::downloadRequest");
	string page;
	//setSegment(s);
	string mylink = getLink(seg);
	page=doGet(mserv+"/mc/"+mylink);
	if (page.find("<div class=\"alertimg\"></div><h3>Virus detected</h3>") > 1)
	{
		std::cout << "Blad: Virus detected" <<endl;
		return 1;
	}
	else
	{
		regex re("class=\"imgname\" href=\"([^\"]*)\"");
		smatch match;
		//regex_search(page,match,re);
		//std::cout << "/n link do strony 0 : " << match[1] <<std::endl;
		//page=doGet(mserv+match[1]);
		//re=regex("id=\"download\" href=\"([^\"]*)\"");
		if(regex_search(page,match,re))
		{
			//LOG(Log::Debug,"Download link: "+match[1]);
			//LOG_LEAVE("Download link found.");
			//setState(Mailbox::DownloadIP);
			//std::cout << "link do strony: " << mserv << match[1] << std::endl << std::endl;
			downloadSeg();
			doGet(mserv+match[1]);
			if (downloadSegDone() == 0)
				return 0;
			else
				return 1;
		}
	}
}

YahooMailbox::~YahooMailbox()
{
}

void YahooMailbox::parseResponse()
{
	//const Mailbox::State state = getState();
	//if (state == Mailbox::LoginDone) // login was requested
	//{
	//}
	//else if (state == Mailbox::ReadHeadersIP) // headers requested
	//{
	//}
}

