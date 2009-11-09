#include "LibMoor.h"

CLibMoor::CLibMoor() {
}

CLibMoor::~CLibMoor() {
}

int CLibMoor::Dehash(std::string HashCode) {
    myHash = new MoorhuntHash(HashCode);
	if (myHash->isValid() != 1)
		return 1;
	else
		return 0;
}

int CLibMoor::DehashYgoow(std::string HashCode) {
	myYgoowHash = new YgoowHash(HashCode);
	
}
int CLibMoor::selectMailBox(int MailBox) {
	int selected = MailBox * 3;
//	myMailBox = new CMailBox(myHash->getAccounts().at(selected - 3), myHash->getAccounts().at(selected-2), myHash->getAccounts().at(selected-1));

	bool cont = false;
	
	int vector_size = myHash->getAccounts().size();
	int j = 0;
	while (cont != true) {
		for (int i = 0; i < vector_size; i+= 3) {
			cout << j;
			std::cout << ". ID: "  << myHash->getAccounts().at(i);
			std::cout << " L: " << myHash->getAccounts().at(i+1);
			std::cout << " P: " << myHash->getAccounts().at(i+2);
			std::cout << std::endl;
			j++;
		}
		std::cout << "Logowanie do: "  << myHash->getAccounts().at(selected) << endl;
		string login = myHash->getAccounts().at(selected+1);
		string passwd = myHash->getAccounts().at(selected+2);
		
		if (myHash->getAccounts().at(selected) == "mail.ru") {
			myMailBox = new MailRuMailbox(login, passwd);
		}
		
		
		if (myMailBox -> Login() == 0) {
			cout << "Zalogowano pomyslnie..." << endl;
			cout << "Sprawdzanie listy segmentow..." << endl;
			int segments = myMailBox -> getHeadersRequest();
			if (segments == 0) {
				cout << "Nie znaleziono zadnego segmentu..." << endl;
				cont = false;
			} else if (segments >= myHash->getNumOfSegments()) {
				cout << "Znaleziono wszystkie segmenty, zaczynam pobieranie" << endl;
				cont = true;
			} else {
				cout << "Znaleziono " << segments << "/" << myHash->getNumOfSegments() << " segmentow. Kontynuowac? " << endl;
				cont = true;
			}
		} else {
			cout << "Logowanie nie powiodlo sie..." << endl;
			cont = false;
			break;
		}
	}
			
	
//	myMailBox -> Login();
	return 0;
}

