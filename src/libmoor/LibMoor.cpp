#include "LibMoor.h"

#include <iostream>

#include "Log.h"
#include "MailboxFactory.h"
#include "HashManager.h"

const int segDownloadTries = 2;

CLibMoor::CLibMoor()
	: mySeg(0),
		selected(0),
		downloadDone(false)
{
}

CLibMoor::~CLibMoor() { }

bool CLibMoor::Dehash(const std::string& hashcode) {
	myHash = boost::shared_ptr<Hash>(HashManager::fromString(hashcode));
	if (myHash->getInfo().valid) {
		int vector_size = myHash->getInfo().accounts.size();
		for (int i = 0; i < vector_size; ++i) {
			std::cout << i << " " << myHash->getInfo().accounts[i].name << std::endl;
		}
	}

	return myHash->getInfo().valid;
}

int CLibMoor::selectMailBox(int MailBox, std::string path) {
	if((path.find_last_of("/") != 0) && (path.length() > 1))
		path += "/";
	
	LOG(Log::Info, boost::format("Pobieranie do %1%") %path);
	std::string strfile = path + myHash->getInfo().fileName;
	std::ifstream myfile (strfile.c_str(), std::ifstream::binary);
	if (boost::filesystem::exists(strfile)) {
		int filesize = boost::filesystem::file_size(strfile);
		if (filesize < myHash->getInfo().fileSize) {
			mySeg = filesize / myHash->getInfo().segmentSize;
			LOG(Log::Info, boost::format("Kontynuuje pobieranie pliku: %1%   Segment: %2%") %strfile %mySeg);
		}
		else {
			LOG(Log::Info, "Plik pobrano w calosci, przerywam...");
			return 1;
		}
	}

	int vector_size = myHash->getInfo().accounts.size();
	/*for (int i = 0; i < vector_size; ++i) {
		LOG(Log::Debug, boost::format( "%1%. ID: %2% L: %3% P: %4%" ) %i
										%myHash->getInfo().accounts[i].name
										%myHash->getInfo().accounts[i].login
										%myHash->getInfo().accounts[i].password);
	}*/

	int tries = 1;
	while (tries <= myHash->getInfo().accounts.size()) {
		std::string mailbox = myHash->getInfo().accounts[selected].name;
		std::string login = myHash->getInfo().accounts[selected].login;
		std::string passwd = myHash->getInfo().accounts[selected].password;
		
//		validMailbox = true;
		
		myMailBox = MailboxFactory::Instance().Create(mailbox, login, passwd);
		if (myMailBox) {
			LOG(Log::Info, boost::format( "Logowanie do:  %1%" )
			               %myHash->getInfo().accounts[selected].name);
			if (myMailBox->loginRequest() == 0) {
				myMailBox->setFileName(path + myHash->getInfo().fileName);
				myMailBox->setFileCRC(myHash->getInfo().crc);
				LOG(Log::Info, "Zalogowano pomyslnie...");
				LOG(Log::Info, "Sprawdzanie listy segmentow...");
				myMailBox->getHeadersRequest();
				unsigned int segments = myMailBox->countAvailableSegments(mySeg);
				if (segments == 0) {
					LOG(Log::Info, "Found none new segments.");
				}
				else {
					LOG(Log::Info, "Found new segments. Downloading...");
					if (startDownload() == 0) {
						LOG(Log::Info, boost::format("Pobranie segmentu %1% nie powiodlo sie... Przelaczanie skrzynki...") %mySeg );
					}
				}
			} 
			else {
				LOG(Log::Info, "Logowanie nie powiodlo sie..." );
			}
		}
		// Program should never reach this execution path, if decoder is
		// implemented properly!
		else {
			LOG(Log::Info, "Wybrana skrzynka nie jest obsługiwana w tej wersji programu.");
		}

		if (tries >= myHash->getInfo().accounts.size()) {
			LOG(Log::Info, "Nie udalo sie pobrac pliku z zadnej ze skrzynek... Koncze program." );
			downloadDone = true;
			delete myMailBox;
			break;
		}

		if (++selected >= vector_size)
				selected = 0;
		++tries;
	}
			
//	myMailBox -> Login();
	return 0;
}

int CLibMoor::startDownload() {
	bool segValid = true;
        while (segValid && (mySeg < myHash->getInfo().numOfSegments)) {
		mySeg++;
		LOG(Log::Info, boost::format( "Sciaganie segmentu: %1%/%2%" )
		               %mySeg
		               %myHash->getInfo().numOfSegments);
		for (int i = 0, segValid = false; i < segDownloadTries; ++i) {
			if (myMailBox->downloadRequest(mySeg) == 0) {
				segValid = true;
		    break;
			}
		}
	}
	
	if (segValid) {
		LOG(Log::Info, "Wszystkie segmenty sciagnieto pomyslnie...");
		downloadDone = true;
	}
	
	return segValid;
}

Status CLibMoor::getStatus() {
	Status s(mySeg, myMailBox->getSpeed(), myMailBox->getBytesRead(),
	         myHash->getInfo().accounts[selected].name);
	return s;
}
