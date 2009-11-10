#include "LibMoor.h"
#include "Log.h"

CLibMoor::CLibMoor() {
	mySeg = 0;
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
	
	// sprawdzenie ktory segment do sciagniecia...
	
	int vector_size = myHash->getAccounts().size();
	int j = 0;
	while (cont != true) {
		for (int i = 0; i < vector_size; i+= 3) {
//			std::cout << j;
//			std::cout << ". ID: "  << myHash->getAccounts().at(i);
//			std::cout << " L: " << myHash->getAccounts().at(i+1);
//			std::cout << " P: " << myHash->getAccounts().at(i+2);
//			std::cout << std::endl;
			LOG( Log::Debug, boost::format( "%1%. ID: %2% L: %3% P: %4%" ) 
			%j %myHash->getAccounts().at(i) %myHash->getAccounts().at(i+1) %myHash->getAccounts().at(i+2) );
			j++;
		}
//		std::cout << "Logowanie do: "  << myHash->getAccounts().at(selected) << endl;
		LOG( Log::Info, boost::format( "Logowanie do:  %1%" ) %myHash->getAccounts().at(selected));
		string login = myHash->getAccounts().at(selected+1);
		string passwd = myHash->getAccounts().at(selected+2);
		
		if (myHash->getAccounts().at(selected) == "mail.ru") {
			myMailBox = new MailRuMailbox(login, passwd);
		}
		myMailBox -> setFileName(myHash->getFileName());
		
		
		if (myMailBox -> Login() == 0) {
//			cout << "Zalogowano pomyslnie..." << endl;
//			cout << "Sprawdzanie listy segmentow..." << endl;
			LOG( Log::Info, "Zalogowano pomyslnie...");
			LOG( Log::Info, "Sprawdzanie listy segmentow...");
			int segments = myMailBox -> getHeadersRequest();
			if (segments == 0) {
//				cout << "Nie znaleziono zadnego segmentu..." << endl;
				LOG( Log::Info, "Nie znaleziono zadnego segmentu...");
				cont = false;
			} else if (segments >= myHash->getNumOfSegments()) {
//				cout << "Znaleziono wszystkie segmenty, zaczynam pobieranie" << endl;
				LOG( Log::Info, "Znaleziono wszystkie segmenty, zaczynam pobieranie");
				cont = true;
				startDownload();
			} else {
//				cout << "Znaleziono " << segments << "/" << myHash->getNumOfSegments() << " segmentow. Kontynuowac? " << endl;
				LOG( Log::Info, boost::format( "Znaleziono %1%/%2% segmentow. Kontynuowac? " ) %segments %myHash->getNumOfSegments());
				cont = true;
			}
		} else {
//			cout << "Logowanie nie powiodlo sie..." << endl;
			LOG( Log::Info, "Logowanie nie powiodlo sie..." );
			cont = false;
			break;
		}
	}
			
	
//	myMailBox -> Login();
	return 0;
}

void CLibMoor::startDownload() {
	while (mySeg < myHash->getNumOfSegments()) {
		mySeg++;
//		cout << "Sciaganie segmenu: " << mySeg << "/" << myHash->getNumOfSegments() << endl;
		LOG( Log::Info, boost::format( "Sciaganie segmentu: %1%/%2%" ) %mySeg %myHash->getNumOfSegments());
		myMailBox -> downloadRequest(mySeg);
	}
//	cout << "Wszystkie segmenty sciagniete..." << endl;
	LOG( Log::Info, "Wszystkie segmenty sciagniete..." );
}
