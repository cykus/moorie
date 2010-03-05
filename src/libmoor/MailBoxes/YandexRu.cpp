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

#include "YandexRu.h"
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
    return new YandexRuMailbox(name, username, password);
  }

  const bool registered = MailboxFactory::Instance().
                                          Register("yandex.ru", Create); //!< rejestracja danej skrzynki
}

YandexRuMailbox::YandexRuMailbox(const std::string &name, const std::string &usr, const std::string &passwd)
    : CMailBox(name, usr, passwd),
      totalEmails( 0 )
{
}

int YandexRuMailbox::loginRequest()
{
	const std::string vars = std::string("login=")
		+ escape(getUser()) + "&passwd="
                + escape(getPassword()) + "&twoweeks=yes"; //!< parametry dla strony logowania

        page = doPost("http://passport.yandex.ru/passport?mode=auth", vars,true); //!< logowanie do skrzynki, zmienna page zawiera stronę główną skrzynki
        boost::smatch match;

	boost::regex re("mode=logout");
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

void YandexRuMailbox::logoutRequest()
{
	/*
	setState(Mailbox::Disconnected); //TODO
	*/
}

void YandexRuMailbox::getHeadersRequest()
{
	std::string url("http://mail.yandex.ru/neo/messages?page_number=1"); //!< adres strony głównej skrzynki
	page = doGet(url); //!< Pobranie zawartości strony

// 	std::cout << page << std::endl;
	int msgcnt = 0; //!< number of message headers for current page
	int pgcnt = 1;//!< numer strony jak przypuszczam
	
	std::stringstream numstr;
	boost::match_results<std::string::const_iterator> match;

	// sprawdzamy czy mamy kolejna strone do wylistowania
	boost::regex re2("id=\"nextPage\" href=\".*?\">.*?<");
	boost::smatch match2;
	
	// a tu regex dla naglowkow
// 	<a class="b-grid__message" href="/neo/message?current_folder=2000000790009768249&amp;ids=1680130000202758563"><span class="b-grid__prefix" title="BrCEJ140 qDq BnEEBAqA"></span>
	boost::smatch match3;

	// span class="b-grid__subject" title="BrCEJ140 qDq BnEEBAqA">
	do  {
		// pobieramy kolejna strone 
		std::stringstream ss;
		ss << pgcnt;
		url = "http://mail.yandex.ru/neo/messages?page_number=" + ss.str(); 
		page = doGet(url);
		
		// zbieranie naglowkow maili
		boost::regex mheadre("<a class=\"b-grid__message\" href=\"(.*?)\"><span class=\"b-grid__prefix\" title=\"(.*?)\"><");
		std::string::const_iterator pbegin = page.begin();
		std::string::const_iterator pend = page.end();
		// petelka do koszenia :)
		while (boost::regex_search(pbegin, pend, match, mheadre, boost::match_default))  { //!< wyszukawanie kolejnych nagłówków
			EmailHeader hdr(match[1], match[2]);
			addHeader(hdr); //!< dodanie do kolejki danych o segmencie
			addHeaderLink(match[1]);
			pbegin = match[2].second;
			++msgcnt;
		}
		
		// kolejna strona
		pgcnt++;
	} while (boost::regex_search(page,match2,re2));
	
	/*
	boost::regex re3("<td width=10% class=\"rovn tdl\"><img src=http://img.yandex.ru/0.gif width=24 height=1><br>(.*?)</td>");
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
                std::string url = "http://win.yandex.ru/cgi-bin/msglist?folder=0&page="+numstr.str(); //!< adres trony do pobrania
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

int YandexRuMailbox::downloadRequest(int seg)
{
	std::string mylink = getLink(seg); //!< pobranie adresu wiadomości zawierającej dany segment
//        setCookie(auth); //!< wstawienie cookie do nagłówka
	boost::erase_all(mylink, "amp;");
     page = doGet("http://mail.yandex.ru"+mylink); //!< pobranie strony zawierającej segment

	//http://mail.yandex.ru/message_part/iran13.doc?hid=1.2&amp;mid=389.120790139.40218083269096258142880460736&amp;ids=1680130000202650296&amp;suid=120790139&amp;name=iran13.doc&amp;filetype=doc"><img class=
	boost::regex re("<div class=\"b-message-attachments__file  att\"><a href=\"(.*?)\"><img class=");
	boost::smatch match;
	std::string link;
	if (boost::regex_search(page,match,re)) //!< wyszukiwanie załącznika
	{
		link=match[1];
		boost::erase_all(link, "amp;");
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

}

int YandexRuMailbox::uploadRequest(std::string filename, std::vector<std::string> to, int seg) {

	return 0;
}


YandexRuMailbox::~YandexRuMailbox()
{
}

void YandexRuMailbox::parseResponse()
{
// 	const Mailbox::State state = getState();
}
