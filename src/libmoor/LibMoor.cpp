#include "LibMoor.h"

CLibMoor::CLibMoor() {
}

CLibMoor::~CLibMoor() {
}

int CLibMoor::Dehash(std::string HashCode) {
    myHash = new MoorhuntHash(HashCode);
	if (myHash->isValid() != 1)
		return 1;

	int vector_size = myHash->getAccounts().size();
	for (int i = 0; i < vector_size; i+= 3) {
		std::cout << "ID: "  << myHash->getAccounts().at(i);
		std::cout << " L: " << myHash->getAccounts().at(i+1);
		std::cout << " P: " << myHash->getAccounts().at(i+2);
		std::cout << std::endl;
	}

// 	for (std::list<std::string>::iterator it = myHash->getAccounts().begin(); it != myHash->getAccounts().end(); it++)
// 		std::cout << (*it) << " ";

	return 0;
}

int CLibMoor::selectMailBox(int MailBox) {
//	int selected = MailBox * 3;
//	myMailBox = new CMailBox(myHash->getAccounts().at(selected - 3), myHash->getAccounts().at(selected-2), myHash->getAccounts().at(selected-1));

// 	myMailBox -> Create();
	return 0;
}

