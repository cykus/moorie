#ifndef _LIBMOOR_H_
#define _LIBMOOR_H_

#include <string>

#include "Hash.h"
#include "Mailbox.h"
#include "Status.h"

class CLibMoor {
	public:
		CLibMoor();
		~CLibMoor();

	public:
		bool Dehash(const std::string& hashcode);
		int selectMailBox(int MailBox, std::string path = "");
		int startDownload();
		Status getStatus();

	private:
		boost::shared_ptr<Hash> myHash;
		CMailBox* myMailBox;
		int mySeg;
		int selected;
		bool downloadDone;
};

#endif
