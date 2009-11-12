#ifndef _LIBMOOR_H_
#define _LIBMOOR_H_

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

#include "MoorhuntHash.h"
#include "YgoowHash.h"
#include "Mailbox.h"
#include "Mailboxes.h"

class CLibMoor {
    MoorhuntHash * myHash;
	YgoowHash * myYgoowHash;
	CMailBox * myMailBox;
	int mySeg;
	
    public:
	CLibMoor();
	~CLibMoor();
	int Dehash(std::string HashCode);
	int DehashYgoow(std::string HashCode);
	int selectMailBox(int MailBox);
	int startDownload();
};

#endif
