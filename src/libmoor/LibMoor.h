#ifndef _LIBMOOR_H_
#define _LIBMOOR_H_

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

#include "Hash.h"
#include "Mailbox.h"
#include "Status.h"

class CLibMoor {
	public:
		CLibMoor();
		~CLibMoor();

	public:
		bool Dehash(const std::string& hashcode);
		int selectMailBox(int MailBox, std::string = "");
		int startDownload();
                bool downloadDone;
		Status getStatus();

	private:
		boost::shared_ptr<Hash> myHash;
		CMailBox* myMailBox;
		int mySeg;
                int selected;
};

#endif
