/*
 * This file is a part of Moorie
 *
 * Copyright (C) 2007-2010 Moorie Team - http://moorie.pl/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "GoogleApps.h"
#include "../Log.h"
#include "../MailboxFactory.h"

#include <sstream>
#include <boost/regex.hpp>
#include <boost/format.hpp>

namespace {
  CMailBox* Create(const std::string& name,
                   const std::string& username,
                   const std::string& password)
  {
    return new GoogleAppsMailbox(name, username, password);
  }
}

const std::string c_names[] = {
	"gala.net",
	"gazeta.pl",
	"mailbox.hu",
	"vivapolska.tv",
	"gde.ru",
	"klikni.cz",
	"livedoor.com",
	"oneindia.in",
	"bigmir.net",
	"sify.com",
	std::string()
};
const bool registered = MailboxFactory::Instance().Register(c_names, Create);

GoogleAppsMailbox::GoogleAppsMailbox(const std::string &name, const std::string &usr, const std::string
&passwd)
  : CMailBox(name, usr, passwd),
  totalEmails( 0 )
{
}

int GoogleAppsMailbox::loginRequest()
{
    boost::smatch match2;


    page = doGet("https://www.google.com/a/"+getMailbox());
    boost::regex re3("name=\"GALX\"[\n].*?value=\"([a-zA-Z0-9_*[-]*]*)\"");
    boost::regex_search(page, match2, re3);
    LOG(Log::Info, boost::format("match: %1% ") %match2[1]);
    const std::string vars = std::string("ltmpl=default&ltmplcache=2&continue=")
                + escape("https://mail.google.com/a/"+getMailbox()+"/")
                +"&service=mail&GALX="
                + escape(match2[1])
                +"&rm=false&hl=pl&Email="+escape(getUser())
                +"&Passwd="+escape(getPassword())
                +"&rmShown=1";
    page = doPost("https://www.google.com/a/"+getMailbox()+"/LoginAction2?service=mail",vars,true);

    //LOG(Log::Info,"page :"+ page);
		std::string username = getUser();
        boost::regex re(username);
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

void GoogleAppsMailbox::logoutRequest()
{
//	setState(Mailbox::Disconnected); //TODO
}

void GoogleAppsMailbox::getHeadersRequest()
{
//	LOG_ENTER( "GoogleAppsMailbox::getHeadersRequest " + getMailbox() );
        std::string url("https://mail.google.com/a/"+getMailbox()+"/?AuthEventSource=SSO&husr="+escape(getUser()+"@"+getMailbox())+"&ui=html&auth=");
//	setState(Mailbox::ReadHeadersIP); // request headers
	totalEmails = 0;
	page = doGet(url + auth);

	int msgcnt = 0; // number of message headers for current page

	boost::match_results<std::string::const_iterator> match;
	boost::regex mheadre("type=\"checkbox\".+?value=\"(.+?)\".+?<a.+?(?:</font>)+\\s*(?:<b>)*(.+?)(?:<)");

	while(regex_search(page,match,mheadre)) {
	//	const string page = getPage();
		std::string::const_iterator pbegin = page.begin();
		std::string::const_iterator pend = page.end();
		msgcnt = 0;
		while (boost::regex_search(pbegin, pend, match, mheadre, boost::match_default))
		{
			EmailHeader hdr(match[1], match[2]);
// 			LOG(Log::Debug, "Found header: " + hdr.subject);
 			addHeader(hdr);
			addHeaderLink(match[1]);
			pbegin = match[2].second;
			++msgcnt;
		}
		if (msgcnt == 0)
		{
//			LOG(Log::Debug, format("Total headers = %d") % totalEmails);
// 			setState(Mailbox::ReadHeadersDone);
		}
		else
		{
			std::stringstream numstr;
			totalEmails += msgcnt;
			numstr << totalEmails;
                        std::string url = "https://mail.google.com/a/"+getMailbox()+"/?ui=html&st=" + numstr.str();
			page = doGet(url);
		}
	}
//	setState(Mailbox::ReadHeadersDone);
}

int GoogleAppsMailbox::downloadRequest(int seg)
{
        std::string mylink = getLink(seg);
//	page = doGet();
        std::string link = "https://mail.google.com/a/"+getMailbox()+"/?realattid=file0&attid=0.1&disp=attd&view=att&th=" + mylink;
		LOG(Log::Debug, link);
        downloadSeg();
        doGet(link);
        if (downloadSegDone() == 0) return 0;
        else return 1;
}

int GoogleAppsMailbox::uploadRequest(std::string filename, std::vector<std::string> to, int seg) {
	std::string segCRC = getSegCRC(filename);

std::cout<<"ok";
        url = "https://mail.google.com/a/"+getMailbox()+"/h/?v=b&pv=tl&cs=b";

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
		
	std::string to_m;
	std::cout<<"ok";
	for (int i = 0; i < to.size(); ++i)
	{
		 to_m+=to[i]+";";
	}
	std::cout<<"ok2";
	addPostData("filename",filename);
	addPostData("to",to_m);
	addPostData("subject",EncodeHeader(filename, segCRC, getFileCRC(), seg));
	addPostData("body","tresc wiadomosci");
	addPostData("nvp_bu_send",to_m);
	addPostData("to","Wy�ij");
	addPostData("filename",filename);
	page = doHTTPUpload(postlink, filename, true);

	return 0;
}

GoogleAppsMailbox::~GoogleAppsMailbox()
{
}

void GoogleAppsMailbox::parseResponse()
{
//	const Mailbox::State state = getState();
}

