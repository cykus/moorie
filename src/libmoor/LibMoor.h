#ifndef _LIBMOOR_H_
#define _LIBMOOR_H_

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

#include "HashInfo.h"
#include "Mailbox.h"
#include "Status.h"

class CLibMoor {
	public:
		CLibMoor();
		~CLibMoor();

	public:
		int Dehash(std::string HashCode);
		int selectMailBox(int MailBox, std::string = "");
		int startDownload();
                bool downloadDone;
		Status getStatus();

	private:
		HashInfo myHash;
		CMailBox* myMailBox;
		int mySeg;
                int selected;
};

#endif
