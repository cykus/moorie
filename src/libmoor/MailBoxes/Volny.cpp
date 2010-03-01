/*
 * This file is a part of Moorie
 *
 * Copyright (C) 2007 Moorie Team - http://moorie.pl/
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
    
#include "Volny.h"
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
    return new VolnyMailbox(name, username, password);
  }

  const bool registered = MailboxFactory::Instance().
                                          Register("volny.cz", Create);
}

VolnyMailbox::VolnyMailbox(const std::string &name, const std::string &usr, const std::string &passwd)
  : CMailBox(name,usr, passwd),
    totalEmails(0)
{
}

int VolnyMailbox::loginRequest()
{
//	LOG_ENTER("VolnyMailbox::loginRequest");
	boost::smatch match;
	boost::smatch match2;
	boost::regex re("Location: (http://www.-mail.volny.cz/~.{0,100})\\?orig_host.{0,50}\\nVary:");
    boost::regex adr("Location: (.{0,100}/)login\\.php");
	boost::regex agent_uid("Set-Cookie: (agent_uid.{0,100})");
        page = doGet("http://mail.volny.cz/",true);
        boost::regex_search(page, match, re);
        boost::regex_search(page, match2, agent_uid);
        setCookie(match2[1]+";");
        const std::string vars = std::string("__USER_CLASS=")
		+ "&login=" + getUser()
                + "&domain=volny.cz"
		+ "&passwd=" + getPassword()
                + "&action_login.x=48"
                + "&action_login.y=8";
    //std::cout<<"safas";
    
	page = doPost(match[1], vars,true);

	boost::regex_search(page, match2, adr);
	url="http://www1-mail.volny.cz" + match2[1];
        re.set_expression("Vítejte");
	if (boost::regex_search(page, match, re))
	{
                re.set_expression("<a href=\"list\\.php\\?id\\=(.{0,50})\">Doručené</a");
                //boost::regex_search(page, match, re);
                //url = unescape("list.php?id=" + match[1]);
		//boost::erase_all(url,"&amp;")
                auth = url;
//		LOG(Log::Debug, "auth=" + auth);
		return 0;
	}
	else
	{
		//return 1;
	}
}

void VolnyMailbox::logoutRequest()
{
//	setState(Mailbox::Disconnected); //TODO
}

void VolnyMailbox::getHeadersRequest()
{
//	LOG_ENTER("VolnyMailbox::getHeadersRequest");
    return;
}

int VolnyMailbox::downloadRequest(int seg)
{
 return 1;
}

int VolnyMailbox::uploadRequest(std::string filename, std::vector<std::string> to, int seg) {
        std::string segCRC = getSegCRC(filename);
	boost::smatch match;
	//boost::regex re("<a href=\"list\\.php\\?id\\=(.{,50})\">Doručené</a");
	boost::regex re("<a href=\"(compose.php\\?.{0,50}\")>Napsat zprávu</a>");
	boost::regex_search(page, match, re);
	url = url+match[1];

	page = doGet(url);
	std::string to_m;
	for (int i = 0; i < to.size(); ++i)
	{
		 to_m+=to[i]+",";
	}

	boost::regex re1("\"astore_uid\" value=\"(.{0,50})\">");
	boost::smatch match1;
	std::string astore;
	boost::regex_search(page, match1, re1);
	astore = match1[1];		//return 1;

	postlink = url+"compose.php";
	setCookie("agent_uid="+astore);
	addPostData("file_1", filename);
	addPostData("astore_uid",astore);
	addPostData("re_message_id", "");
	addPostData("re_references", "");
	addPostData("folder", "%0D%0A");
	addPostData("sort", "");
	addPostData("html_mode", "");
	addPostData("last_saved_draft_id", "");
	addPostData("from", "0");
	addPostData("to", to_m);//to
	addPostData("addrbook_to", "");
	addPostData("cc", "");
	addPostData("addrbook_cc", "");
	addPostData("bcc", "");
	addPostData("addrbook_bcc", "");
	addPostData("subject",EncodeHeader(filename, segCRC, getFileCRC(), seg));
	addPostData("priority", "0");
	addPostData("msg", "safsadfsa");
	addPostData("copyfolder", "Sent");
	addPostData("spellcheck_lang", "");
	addPostData("spellcheck", "0");
	addPostData("file_2", "");
	addPostData("file_3", "");
	addPostData("file_4", "");
	addPostData("file_5", "");
	addPostData("file_6", "");
	addPostData("file_7", "");
	addPostData("file_8", "");
	addPostData("file_9", "");
	addPostData("file_10", "");
	addPostData("attach.x", "70");
	addPostData("attach.y", "1");
	
	page = doHTTPUpload(postlink, filename, true);
	
	re.set_expression("name=\"fs_md5\".{0,20}<option value=\"(.{0,40})\">");
	boost::regex_search(page, match, re);
	std::string fs_md5 = match[1];
	
	setCookie("agent_uid="+astore);

	addPostData("","");
	addPostData("astore_uid",astore);
	addPostData("re_message_id", "");
	addPostData("re_references", "");
	addPostData("folder", "%0D%0A");
	addPostData("sort", "");
	addPostData("html_mode", "");
	addPostData("last_saved_draft_id", "");
	addPostData("from", "");
	addPostData("to", to_m);//to
	addPostData("addrbook_to", "");
	addPostData("cc", "");
	addPostData("addrbook_cc", "");
	addPostData("bcc", "");
	addPostData("addrbook_bcc", "");
	addPostData("subject", EncodeHeader(filename, segCRC, getFileCRC(), seg));
	addPostData("priority", "0");
	addPostData("msg", "safsadfsa");
	addPostData("copyfolder", "Sent");
	addPostData("send.x", "50");
	addPostData("send.y", "10");
	addPostData("spellcheck_lang", "");
	addPostData("spellcheck", "0");
	addPostData("file_1", "");
	addPostData("file_2", "");
	addPostData("file_3", "");
	addPostData("file_4", "");
	addPostData("file_5", "");
	addPostData("file_6", "");
	addPostData("file_7", "");
	addPostData("file_8", "");
	addPostData("file_9", "");
	addPostData("file_10", "");
	addPostData("fs_md5", fs_md5);

	
	page = doHTTPUpload(postlink, "", true);
	return 0;
}

VolnyMailbox::~VolnyMailbox()
{
}

void VolnyMailbox::parseResponse()
{
//	const Mailbox::State state = getState();
}

