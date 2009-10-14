#ifndef _LIBMOOR_H_
#define _LIBMOOR_H_

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

#include "MoorhuntHash.h"
#include "Mailboxes.h"

class CLibMoor {
    MoorhuntHash * myHash;
    static short int SelectedMailBox;

    public:
	CLibMoor();
	~CLibMoor();
	int Dehash(std::string HashCode);
};

#endif
