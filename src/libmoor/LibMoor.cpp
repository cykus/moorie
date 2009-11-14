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
	else {	
		int j = 0;
		int vector_size = myHash->getAccounts().size();
	
		for (int i = 0; i < vector_size; i+= 3) {
			cout << j << " " << myHash->getAccounts().at(i) << endl;
//			LOG( Log::Debug, boost::format( "%1%. ID: %2% L: %3% P: %4%" ) 
//					%j %myHash->getAccounts().at(i) %myHash->getAccounts().at(i+1) %myHash->getAccounts().at(i+2) );
			j++;
		}
		return 0;
	}
}

int CLibMoor::DehashYgoow(std::string HashCode) {
	myYgoowHash = new YgoowHash(HashCode);
}
int CLibMoor::selectMailBox(int MailBox, std::string path) {
	if(path.find_last_of("/") != 0) {
		path+="/";
	}
	LOG( Log::Info, boost::format("Pobieranie do %1%") %path);
	int selected = MailBox * 3;
//	myMailBox = new CMailBox(myHash->getAccounts().at(selected - 3), myHash->getAccounts().at(selected-2), myHash->getAccounts().at(selected-1));

	bool cont = false;
	bool changeMailbox = false;
	bool validMailbox = true;
	// sprawdzenie ktory segment do sciagniecia...
	
	int vector_size = myHash->getAccounts().size();
	int j = 0;
	int tries = 0;
	
	for (int i = 0; i < vector_size; i+= 3) {
//			std::cout << j;
//			std::cout << ". ID: "  << myHash->getAccounts().at(i);
//			std::cout << " L: " << myHash->getAccounts().at(i+1);
//			std::cout << " P: " << myHash->getAccounts().at(i+2);
//			std::cout << std::endl;
		LOG( Log::Debug, boost::format( "%1%. ID: %2% L: %3% P: %4%" ) %j %myHash->getAccounts().at(i) %myHash->getAccounts().at(i+1) %myHash->getAccounts().at(i+2) );
//		j++;
	}
	
	do {
		// zmiana skrzynki?
		if (changeMailbox == true) {
			selected+= 3;
			if (selected >= vector_size) 
				selected = 0;
			tries++; // zmian skrzynek
		}

//		std::cout << "Logowanie do: "  << myHash->getAccounts().at(selected) << endl;
		LOG( Log::Info, boost::format( "Logowanie do:  %1%" ) %myHash->getAccounts().at(selected));
		string login = myHash->getAccounts().at(selected+1);
		string passwd = myHash->getAccounts().at(selected+2);
		
		if (myHash->getAccounts().at(selected) == "mail.ru") {
			myMailBox = new MailRuMailbox(login, passwd);
			validMailbox = true;
		} else if (myHash->getAccounts().at(selected) == "gmail.com") {
			myMailBox = new GMailMailbox(login, passwd);
			validMailbox = true;
		} else {
			LOG( Log::Info, "Blad skrzynki");
			validMailbox = false;
		}
		
		if (validMailbox == true && myMailBox -> loginRequest() == 0) {
            myMailBox -> setFileName(path+myHash->getFileName());
			myMailBox -> setFileCRC(myHash->getCrc());
//			cout << "Zalogowano pomyslnie..." << endl;
//			cout << "Sprawdzanie listy segmentow..." << endl;
			LOG( Log::Info, "Zalogowano pomyslnie...");
			LOG( Log::Info, "Sprawdzanie listy segmentow...");
			myMailBox -> getHeadersRequest();
			int segments = myMailBox->checkHeaders(myHash->getNumOfSegments());
			if (segments == 0) {
//				cout << "Nie znaleziono zadnego segmentu..." << endl;
				LOG( Log::Info, "Nie znaleziono zadnego segmentu...");
				changeMailbox = true;
			} else if (segments >= myHash->getNumOfSegments()) {
//				cout << "Znaleziono wszystkie segmenty, zaczynam pobieranie" << endl;
				LOG( Log::Info, "Znaleziono wszystkie segmenty, zaczynam pobieranie");
				cont = true;
				if (startDownload() != 0) {
					LOG( Log::Info, boost::format("Pobranie segmentu %1% nie powiodlo sie... Przelaczanie skrzynki...") %segments );
					changeMailbox = true;
				} 
			} else {
//				cout << "Znaleziono " << segments << "/" << myHash->getNumOfSegments() << " segmentow. Kontynuowac? " << endl;
				LOG( Log::Info, boost::format( "Znaleziono %1%/%2% segmentow. Kontynuowac? " ) %segments %myHash->getNumOfSegments());
				cont = true;
			}
		} else {
//			cout << "Logowanie nie powiodlo sie..." << endl;
			LOG( Log::Info, "Logowanie nie powiodlo sie..." );
//			cont = false;
//			break;
			changeMailbox = true;
		}
		
		if (tries >= vector_size/3) {
			LOG( Log::Info, "Nie udalo sie pobrac pliku z zadnej ze skrzynek... Koncze program." );
			delete myMailBox;
			break;
		}
	} while (cont != true);
			
	
//	myMailBox -> Login();
	return 0;
}

int CLibMoor::startDownload() {
	bool segValid;
	int seg_left = myHash->getNumOfSegments();
	do {
		mySeg++;
//		cout << "Sciaganie segmenu: " << mySeg << "/" << myHash->getNumOfSegments() << endl;
		LOG( Log::Info, boost::format( "Sciaganie segmentu: %1%/%2%" ) %mySeg %seg_left);
		if (myMailBox -> downloadRequest(mySeg) != 0) {
			if (myMailBox -> downloadRequest(mySeg) != 0) {
				segValid = 0;
				break;
			}
			else 
				segValid = 1;// proba pobrania segmentu jeszcze raz
		} else
			segValid = 1;
	} while (mySeg < seg_left);
	if (segValid == 1)
		LOG( Log::Info, "Wszystkie segmenty sciagnieto pomyslnie...");
	return segValid;
}
