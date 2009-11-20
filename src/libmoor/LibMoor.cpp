#include "LibMoor.h"
#include "Log.h"
#include "MailboxFactory.h"
#include "HashManager.h"

CLibMoor::CLibMoor() {
	mySeg = 0;
        selected = 0;
        downloadDone = false;
}

CLibMoor::~CLibMoor() {
	
}

int CLibMoor::Dehash(std::string HashCode) {
  myHash = boost::shared_ptr<Hash>(HashManager::fromString(HashCode));
	if (!myHash->getInfo().valid)
		return 1;
	else {	
		int j = 0;
		int vector_size = myHash->getInfo().accounts.size();
	
		for (int i = 0; i < vector_size; i+= 3) {
			std::cout << j << " " << myHash->getInfo().accounts.at(i) << std::endl;
//			LOG( Log::Debug, boost::format( "%1%. ID: %2% L: %3% P: %4%" ) 
//					%j %myHash->getAccounts().at(i) %myHash->getAccounts().at(i+1) %myHash->getAccounts().at(i+2) );
			j++;
		}
		return 0;
	}
}

int CLibMoor::selectMailBox(int MailBox, std::string path) {
        if(path.find_last_of("/") != 0 && path.length() > 1) path+="/";
	LOG( Log::Info, boost::format("Pobieranie do %1%") %path);

	// sprawdzanie pobranego pliku
	string strfile = path + myHash->getInfo().fileName;
	ifstream myfile (strfile.c_str(), std::ifstream::binary);
	if (boost::filesystem::exists(strfile)) {
		int filesize = boost::filesystem::file_size(strfile);
		if (filesize < myHash->getInfo().fileSize) {
			mySeg = filesize / myHash->getInfo().segmentSize;
			LOG(Log::Info, boost::format("Kontynuuje pobieranie pliku: %1%   Segment: %2%") %strfile %mySeg);
		} else  {
                        LOG(Log::Info, "Plik pobrano w calosci, przerywam...");
			return 1;
		}
			
	}
	
//	myMailBox = new CMailBox(myHash->getAccounts().at(selected - 3), myHash->getAccounts().at(selected-2), myHash->getAccounts().at(selected-1));

	bool cont = false;
	bool changeMailbox = false;
	bool validMailbox = true;
	// sprawdzenie ktory segment do sciagniecia...
	
	int vector_size = myHash->getInfo().accounts.size();
	int j = 0;
	int tries = 0;
	
	for (int i = 0; i < vector_size; i+= 3) {
//			std::cout << j;
//			std::cout << ". ID: "  << myHash.accounts.at(i);
//			std::cout << " L: " << myHash.accounts.at(i+1);
//			std::cout << " P: " << myHash.accounts.at(i+2);
//			std::cout << std::endl;
		LOG( Log::Debug, boost::format( "%1%. ID: %2% L: %3% P: %4%" ) %j %myHash->getInfo().accounts.at(i) %myHash->getInfo().accounts.at(i+1) %myHash->getInfo().accounts.at(i+2) );
//		j++;
	}
	
	do {
		// zmiana skrzynki?
		if (changeMailbox == true) {
			selected += 3;
			if (selected >= vector_size) 
				selected = 0;
			tries++; // zmian skrzynek
		}

		std::string mailbox = myHash->getInfo().accounts.at(selected);
		std::string login = myHash->getInfo().accounts.at(selected + 1);
		std::string passwd = myHash->getInfo().accounts.at(selected + 2);
		
		validMailbox = true;
		
		myMailBox = MailboxFactory::Instance().Create(mailbox, login, passwd);
		if (!myMailBox) {
			LOG(Log::Info, "Blad skrzynki");
			validMailbox = false;
		}
		
		LOG(Log::Info, boost::format( "Logowanie do:  %1%" )
		               %myHash->getInfo().accounts.at(selected));

		if (validMailbox == true && myMailBox -> loginRequest() == 0) {
			myMailBox->setFileName(path+myHash->getInfo().fileName);
			myMailBox->setFileCRC(myHash->getInfo().crc);
			LOG(Log::Info, "Zalogowano pomyslnie...");
			LOG(Log::Info, "Sprawdzanie listy segmentow...");
			myMailBox->getHeadersRequest();
			int segments = myMailBox->checkHeaders(myHash->getInfo().numOfSegments);
			if (segments == 0) {
				LOG(Log::Info, "Nie znaleziono zadnego segmentu...");
				changeMailbox = true;
			} else if (segments >= myHash->getInfo().numOfSegments) {
				LOG(Log::Info, "Znaleziono wszystkie segmenty, zaczynam pobieranie");
				cont = true;
				if (startDownload() == 0) {
					LOG(Log::Info, boost::format("Pobranie segmentu %1% nie powiodlo sie... Przelaczanie skrzynki...") %segments );
					changeMailbox = true;
				} 
			} else {
				LOG(Log::Info, boost::format( "Znaleziono %1%/%2% segmentow. Kontynuowac? " ) %segments %myHash->getInfo().numOfSegments);
				cont = true;
			}
		} else {
			LOG(Log::Info, "Logowanie nie powiodlo sie..." );
			changeMailbox = true;
		}
		
		if (tries >= (vector_size / 3)) {
			LOG(Log::Info, "Nie udalo sie pobrac pliku z zadnej ze skrzynek... Koncze program." );
                        downloadDone = true;
                        delete myMailBox;
			break;
		}
	} while (cont != true);
			
//	myMailBox -> Login();
	return 0;
}

int CLibMoor::startDownload() {
	bool segValid;
	int seg_left = myHash->getInfo().numOfSegments;
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
        if (segValid == 1){
		LOG( Log::Info, "Wszystkie segmenty sciagnieto pomyslnie...");
                downloadDone = true;
        }
	return segValid;
}
Status CLibMoor::getStatus() {
    Status s(mySeg, myMailBox->getSpeed(), myMailBox->getBytesRead(), myHash->getInfo().accounts.at(selected));
    return s;
}
