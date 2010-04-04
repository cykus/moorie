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

  const bool registered = MailboxFactory::Instance().
                                          Register("interia.pl", Create); //!< rejestracja danej skrzynki
}

InteriaPlMailbox::InteriaPlMailbox(const std::string &name, const std::string &usr, const std::string &passwd)
    : CMailBox(name, usr, passwd),
      totalEmails( 0 )
{
}

int InteriaPlMailbox::loginRequest()
{
	const std::string vars = std::string("referer=&email=")
			+ escape(getUser()) + escape("@interia.pl") + "&pass="
			+ escape(getPassword()) + "&formHTTP=1&webmailSelect=classicMail"; //!< parametry dla strony logowania

	std::string address = "https://ssl.interia.pl/login.html?webmailSelect=classicMail";
	page = doPost(address, vars,true); //!< logowanie do skrzynki, zmienna page zawiera stronę główną skrzynki
        boost::smatch match;

		std::cout << vars << std::endl;
		std::cout << page << std::endl;

		boost::regex re("wyloguj");
		if (boost::regex_search(page, match, re))
		{
			auth = match[1];
			return 0; //!< Jeśli logowanie zakończyło sie powodzeniem
		}
		else
		{
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
	/*
        std::string url("http://win.mail.ru/cgi-bin/msglist?folder=0"); //!< adres strony głównej skrzynki
	//setState(Mailbox::ReadHeadersIP); // request headers
// 	totalEmails = 0;
//     setCookie(auth);
        page = doGet(url); //!< Pobranie zawartości strony

        int msgcnt = 0; //!< number of message headers for current page
        int pgcnt = 0;//!< numer strony jak przypuszczam
	std::stringstream numstr;
	boost::match_results<std::string::const_iterator> match;

	boost::regex re2("<a href=\".*?\" id=\"nextbut\">.*?\n&nbsp;&nbsp;<a");
	boost::smatch match2;

	boost::regex re3("<td width=10% class=\"rovn tdl\"><img src=http://img.mail.ru/0.gif width=24 height=1><br>(.*?)</td>");
	boost::smatch match3;
	boost::regex_search(page,match3,re3);
	std::istringstream pg(match3[1]);
        int pages; //!< liczba stron z mailami
	pg >> pages;
        pages = pages / 25 + 1;

	LOG(Log::Debug,boost::format( "match: "+match3[1]+" stron: %d" ) % pages );
	while (pgcnt < pages) {
                pgcnt++; //!< zwiększenie numeru strony o jeden
		numstr.str("");
                numstr << pgcnt; //!< konwersja numeru strony do tekstu
                std::string url = "http://win.mail.ru/cgi-bin/msglist?folder=0&page="+numstr.str(); //!< adres trony do pobrania
                page = doGet(url); //!< pobranie kolejnej strony z mailami

		boost::regex mheadre("<td class=letavtor title=.*?</a></td>.*?<td class=lettem><a href=\"readmsg([^\"]*)\"[^<>]*>([^<>]*)</a></td>");
	//	const string page = getPage();
		std::string::const_iterator pbegin = page.begin();
		std::string::const_iterator pend = page.end();
                while (boost::regex_search(pbegin, pend, match, mheadre, boost::match_default)) { //!< wyszukawanie kolejnych nagłówków
                        EmailHeader hdr(match[1], match[2]); //!< pierwszy parametr to unikalne ID wiadomości a drugi to temat wiadomości email
//			cout << match[1] << " " << match[2] << endl;
//			cout << "Add header: " << hdr.subject << endl;
//			LOG(Log::Debug, "Add header: " + hdr.subject + " Link: " + match[1]);
                        addHeader(hdr); //!< dodanie do kolejki danych o segmencie
//			addHeaderSubject(hdr.subject);
                        addHeaderLink(match[1]); //!< dodanie unikalnego ID segmentu do kolejki
			pbegin = match[2].second;
			++msgcnt;
		}
//		LOG(Log::Debug, ".");
	}
	*/

//	setState(Mailbox::ReadHeadersDone); */
}

int InteriaPlMailbox::downloadRequest(int seg)
{
	/*
        std::string mylink = getLink(seg); //!< pobranie adresu wiadomości zawierającej dany segment
//	cout << mylink << endl;
        setCookie(auth); //!< wstawienie cookie do nagłówka
        page = doGet("http://win.mail.ru/cgi-bin/readmsg"+mylink); //!< pobranie strony zawierającej segment
//	regex re("http://a[a-z]*[0-9].mail.ru/cgi-bin/readmsg/.*?&mode=attachment&channel=");
        boost::regex re("<a href=\"(http://win.mail.ru/cgi-bin/getattach[^\"]*&mode=attachment&channel=&notype)");
// 	http://win.mail.ru/cgi-bin/getattach?file=devcpp%2d4.9.9.2_setup.exe.5&id=12643664970000000633;0;1&mode=attachment&channel=&notype
	boost::smatch match;
	std::string link;
        if (boost::regex_search(page,match,re)) //!< wyszukiwanie załącznika
	{
		link=match[1];
//		cout << link << endl;
		LOG(Log::Debug, link);
                downloadSeg();  //!< utworzenie pliku tymczasowego na dysku ?
                doGet(link); //!< pobieranie załącznika (segmentu)
                if (downloadSegDone() == 0) //!< sprawdzenie czy segment został pobrany
			return 0;
		else
			return 1;
//		setState(Mailbox::DownloadIP);
//		setSegment(s);
	}
	*/
}

int InteriaPlMailbox::uploadRequest(std::string filename, std::vector<std::string> to, int seg) {
	return 0;
}


InteriaPlMailbox::~InteriaPlMailbox()
{
}

void InteriaPlMailbox::parseResponse()
{
// 	const Mailbox::State state = getState();
}
