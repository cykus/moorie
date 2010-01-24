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

#include "MailRu.h"
#include "../Log.h"
#include "../MailboxFactory.h"

// #include "Downloader.h"
// #include "MoorieException.h"
#include <sstream>
#include <boost/regex.hpp>

namespace {
  CMailBox* Create(const std::string& username,
                   const std::string& password)
  {
    return new MailRuMailbox(username, password);
  }

  const bool registered = MailboxFactory::Instance().
                                          Register("mail.ru", Create);
}

MailRuMailbox::MailRuMailbox(const std::string &usr, const std::string &passwd)
	: CMailBox(usr, passwd),
		totalEmails(0)
{
}

int MailRuMailbox::loginRequest()
{

//	LOG_ENTER("MailRuMailbox::loginRequest");
	const std::string vars = std::string("Login=")
		+ escape(getUser()) + "&Domain=" + escape("mail.ru") + "&Password="
		+ escape(getPassword()) + "&level=1";
//	setState(Mailbox::LoginIP);
	page = doPost("http://win.mail.ru/cgi-bin/auth", vars,true);
	boost::smatch match;
//	page = doGet("http://win.mail.ru/cgi-bin/msglist?folder=0");

	boost::regex re("Set-Cookie: (Mpop=.*?;)");
	if (boost::regex_search(page, match, re))
	{
		auth = match[1];
//		LOG(Log::Debug, "auth=" + auth);
//		setState(Mailbox::LoginDone); // logged in
		return 0;
	}
	else
	{
//		setState(Mailbox::LoginError); //login failure
//		throw MoorieException("Login failed");
		return 1;
	}
//	setState(Mailbox::Connected);
}

void MailRuMailbox::logoutRequest()
{
	/*
	setState(Mailbox::Disconnected); //TODO
	*/
}

void MailRuMailbox::getHeadersRequest()
{
//	LOG_ENTER("MailRuMailbox::getHeadersRequest");
	std::string url("http://win.mail.ru/cgi-bin/msglist?folder=0");
	//setState(Mailbox::ReadHeadersIP); // request headers
// 	totalEmails = 0;
//     setCookie(auth);
 	page = doGet(url);

	int msgcnt = 0; // number of message headers for current page
	int pgcnt = 0;
	std::stringstream numstr;
	boost::match_results<std::string::const_iterator> match;

	boost::regex re2("<a href=\".*?\" id=\"nextbut\">.*?\n&nbsp;&nbsp;<a");
	boost::smatch match2;

	boost::regex re3("<td width=10% class=\"rovn tdl\"><img src=http://img.mail.ru/0.gif width=24 height=1><br>(.*?)</td>");
	boost::smatch match3;
	boost::regex_search(page,match3,re3);
	std::istringstream pg(match3[1]);
	int pages;
	pg >> pages;
	pages = pages / 25 + 1;
//	cout << "match: " << match3[1] << " stron: " << pages;
	LOG(Log::Debug,boost::format( "match: "+match3[1]+" stron: %d" ) % pages );
	while (pgcnt < pages) {
		pgcnt++;
		numstr.str("");
		numstr << pgcnt;
		std::string url = "http://win.mail.ru/cgi-bin/msglist?folder=0&page="+numstr.str();
		page = doGet(url);

		boost::regex mheadre("<td class=letavtor title=.*?</a></td>.*?<td class=lettem><a href=\"readmsg([^\"]*)\"[^<>]*>([^<>]*)</a></td>");
	//	const string page = getPage();
		std::string::const_iterator pbegin = page.begin();
		std::string::const_iterator pend = page.end();
		while (boost::regex_search(pbegin, pend, match, mheadre, boost::match_default)) {
			EmailHeader hdr(match[1], match[2]);
//			cout << match[1] << " " << match[2] << endl;
//			cout << "Add header: " << hdr.subject << endl;
//			LOG(Log::Debug, "Add header: " + hdr.subject + " Link: " + match[1]);
			addHeader(hdr);
//			addHeaderSubject(hdr.subject);
			addHeaderLink(match[1]);
			pbegin = match[2].second;
			++msgcnt;
		}
//		LOG(Log::Debug, ".");
	}

//	setState(Mailbox::ReadHeadersDone); */
}

int MailRuMailbox::downloadRequest(int seg)
{
	std::string mylink = getLink(seg);
//	cout << mylink << endl;
 	setCookie(auth);
 	page = doGet("http://win.mail.ru/cgi-bin/readmsg"+mylink);
//	regex re("http://a[a-z]*[0-9].mail.ru/cgi-bin/readmsg/.*?&mode=attachment&channel=");
 	boost::regex re("<a href=\"(http://a[^\"]*/cgi-bin/readmsg/[^\"]*&mode=attachment&channel=)");

	boost::smatch match;
	std::string link;
	if (boost::regex_search(page,match,re))
	{
		link=match[1];
//		cout << link << endl;
		LOG(Log::Debug, link);
		downloadSeg();
		doGet(link);
		if (downloadSegDone() == 0)
			return 0;
		else
			return 1;
//		setState(Mailbox::DownloadIP);
//		setSegment(s);
	}

}

int MailRuMailbox::uploadRequest(std::string filename, std::string to) {
	LOG(Log::Debug, boost::format( "uploadRequest" ));

	size_t len, request_length;
	std::string message;
	using boost::asio::ip::tcp;

	len = strlen(getUser().c_str());
	unsigned char *b64login = base64((char*)getUser().c_str(), len);
	len = strlen(getPassword().c_str());
	unsigned char *b64password = base64((char*)getPassword().c_str(), len);

std::cout << b64login << " " << b64password << std::endl;

	boost::asio::io_service io_service;
	tcp::resolver resolver(io_service);
	tcp::resolver::query query("smtp.mail.ru", "smtp");
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::resolver::iterator end;
	tcp::socket socket(io_service);
	boost::system::error_code error = boost::asio::error::host_not_found;

	while (error && endpoint_iterator != end)
	{
		socket.close();
		socket.connect(*endpoint_iterator++, error);
	}
	if (error)
		throw boost::system::system_error(error);

	boost::array<char, 128> buf;

	len = socket.read_some(boost::asio::buffer(buf), error);
	std::cout.write(buf.data(), len);

	// powiedz ladnie HELO
	message = "HELO smtpproxy.netcity.pl\n";
	request_length = strlen(message.c_str());
	boost::asio::write(socket, boost::asio::buffer(message, request_length));
	len = socket.read_some(boost::asio::buffer(buf), error);
	std::cout.write(buf.data(), len);

	// autoryzacja
	message = "AUTH LOGIN\n";
	request_length = strlen(message.c_str());
	boost::asio::write(socket, boost::asio::buffer(message, request_length));
	len = socket.read_some(boost::asio::buffer(buf), error);
	std::cout.write(buf.data(), len);

	// autoryzacja - login
	request_length = strlen((char*)b64login);
	boost::asio::write(socket, boost::asio::buffer(b64login, request_length));
	boost::asio::write(socket, boost::asio::buffer("\n", 1));
	len = socket.read_some(boost::asio::buffer(buf), error);
	std::cout.write(buf.data(), len);

	// autoryzacja - haslo
	request_length = strlen((char*)b64password);
	boost::asio::write(socket, boost::asio::buffer(b64password, request_length));
	boost::asio::write(socket, boost::asio::buffer("\n", 1));
	len = socket.read_some(boost::asio::buffer(buf), error);
	std::cout.write(buf.data(), len);

// nadawca
	message = "MAIL FROM:" + getUser() + "@mail.ru\n";
	request_length = strlen(message.c_str());
	boost::asio::write(socket, boost::asio::buffer(message, request_length));
	len = socket.read_some(boost::asio::buffer(buf), error);
	std::cout.write(buf.data(), len);


	// odbiorca
	message = "RCPT TO: " + getUser() + "@mail.ru\n";
	request_length = strlen(message.c_str());
	boost::asio::write(socket, boost::asio::buffer(message, request_length));
	len = socket.read_some(boost::asio::buffer(buf), error);
	std::cout.write(buf.data(), len);

	// odbiorca
	message = "DATA\n";
	request_length = strlen(message.c_str());
	boost::asio::write(socket, boost::asio::buffer(message, request_length));
	len = socket.read_some(boost::asio::buffer(buf), error);
	std::cout.write(buf.data(), len);

	// odbiorca
	message = "testowanie dupa dupa\n";
	request_length = strlen(message.c_str());
	boost::asio::write(socket, boost::asio::buffer(message, request_length));


		// odbiorca
	message = ".\n";
	request_length = strlen(message.c_str());
	boost::asio::write(socket, boost::asio::buffer(message, request_length));
	len = socket.read_some(boost::asio::buffer(buf), error);
	std::cout.write(buf.data(), len);

	// odbiorca
	message = "quit\n";
	request_length = strlen(message.c_str());
	boost::asio::write(socket, boost::asio::buffer(message, request_length));
	len = socket.read_some(boost::asio::buffer(buf), error);
	std::cout.write(buf.data(), len);

	return 0;
}


MailRuMailbox::~MailRuMailbox()
{
}

void MailRuMailbox::parseResponse()
{
// 	const Mailbox::State state = getState();
}
