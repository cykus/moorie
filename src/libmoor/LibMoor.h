#ifndef _LIBMOOR_H_
#define _LIBMOOR_H_

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <boost/filesystem.hpp>
#include <fstream>

#include "MoorhuntHash.h"
#include "YgoowHash.h"
#include "Mailbox.h"
#include "Mailboxes.h"
#include "Status.h"

class CLibMoor {
    MoorhuntHash * myHash;
	YgoowHash * myYgoowHash;
	CMailBox * myMailBox;
	int mySeg;
        int selected;
	
    public:
	CLibMoor();
	~CLibMoor();
	int Dehash(std::string HashCode);
	int DehashYgoow(std::string HashCode);
        int selectMailBox(int MailBox,std::string = "");
	int startDownload();
        Status getStatus();
};

#endif
