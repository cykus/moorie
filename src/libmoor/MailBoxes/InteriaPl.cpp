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

#include "InteriaPl.h"
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
    return new InteriaPlMailbox(name, username, password);
  }

  const std::string c_names[] = {
	"interia.pl",
   "poczta.fm",
   std::string()
  };
  const bool registered = MailboxFactory::Instance().Register(c_names, Create);
}

InteriaPlMailbox::InteriaPlMailbox(const std::string &name, const std::string &usr, const std::string &passwd)
    : CMailBox(name, usr, passwd),
      totalEmails( 0 )
{
}

int InteriaPlMailbox::loginRequest()
{
	const std::string vars = std::string("referer=&email=")
			+ escape(getUser()) + "@"+getMailbox() + "&pass="
			+ escape(getPassword()) + "&formHTTP=1&webmailSelect=classicMail"; //!< parametry dla strony logowania

	std::string address = "https://ssl.interia.pl/login.html?webmailSelect=classicMail";
	page = doPost(address, vars,true); //!< logowanie do skrzynki, zmienna page zawiera stronę główną skrzynki

	boost::smatch match;
	boost::regex re("/classic/,uid,(.*?)\" >");
	if (boost::regex_search(page, match, re)) {
		auth = match[1];
		return 0; //!< Jeśli logowanie zakończyło sie powodzeniem
	}
	else {
		return 1; //!< Jeśli logowanie zakończyło sie niepowodzeniem
	}
}

void InteriaPlMailbox::logoutRequest()
{
	/*
	setState(Mailbox::Disconnected); //TODO
	*/
}

void InteriaPlMailbox::getHeadersRequest()
{
}

int InteriaPlMailbox::downloadRequest(int seg)
{
	return 1;
}

int InteriaPlMailbox::uploadRequest(std::string filename, std::vector<std::string> to, int seg) {
	std::string segCRC = getSegCRC(filename);
	page = doGet("http://poczta.interia.pl/classic/composemail,uid,"+auth);

// 	std::cout << "MACZ: " <<  auth << std::endl;

	std::string to_m;
	for (int i = 0; i < to.size(); ++i)
	{
		to_m+=to[i]+",";
	}

	boost::regex re1("<input type=\"hidden\" name=\"mailComposeMark\" value=\"(.*?)\"/>");
	boost::smatch match1;
	boost::regex_search(page, match1, re1);
	std::string mark;
	mark = match1[1];		//return 1;

	std::string postlink = "http://poczta.interia.pl/classic/composemail,send,1,uid,"+auth;
	addPostData("fid", "");
	addPostData("midOrg", "");
	addPostData("mpage", "");
	addPostData("mailComposeMark", mark);
	addPostData("mailComposeReceiver", to_m);//to
	addPostData("mailComposeCopy", "");
	addPostData("mailComposeBCopy", "");
	addPostData("mailComposeSubject",EncodeHeader(filename, segCRC, getFileCRC(), seg));
	addPostData("mailComposeFile_1", filename);
	addPostData("mailComposeBody", "jakas tam wiadomosc sobie jest z zalacznikiem, blabla");
	addPostData("mailcomposePrior", "3");
	addPostData("mailcomposeConfirm", "0");
	addPostData("mailComposeSend", "Wy�lij");
	page = doHTTPUpload(postlink, filename, true);
std::cout << page << std::endl;
	return 0;

}


InteriaPlMailbox::~InteriaPlMailbox()
{
}

void InteriaPlMailbox::parseResponse()
{
// 	const Mailbox::State state = getState();
}
