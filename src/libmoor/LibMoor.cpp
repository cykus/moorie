#include "LibMoor.h"

CLibMoor::CLibMoor() {
}

CLibMoor::~CLibMoor() {
}

int CLibMoor::Dehash(std::string HashCode) {
    myHash = new MoorhuntHash(HashCode);
	if (myHash->isValid() != 1)
		return 1;


	return 0;
}

