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
	
	mySeg = getLastSegment(path + myHash->getInfo().fileName);
	if (mySeg == myHash->getInfo().numOfSegments) {
		LOG(Log::Info, "Plik pobrano w calosci, przerywam...");
		return 1;
	}

	int vector_size = myHash->getInfo().accounts.size();
	/*for (int i = 0; i < vector_size; ++i) {
		LOG(Log::Debug, boost::format( "%1%. ID: %2% L: %3% P: %4%" ) %i
										%myHash->getInfo().accounts[i].name
										%myHash->getInfo().accounts[i].login
										%myHash->getInfo().accounts[i].password);
	}*/

	int tries = 1;
	while (tries <= myHash->getInfo().accounts.size() && downloadDone == false) {
		std::string mailbox = myHash->getInfo().accounts[selected].name;
		std::string login = myHash->getInfo().accounts[selected].login;
		std::string passwd = myHash->getInfo().accounts[selected].password;
		
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
						LOG(Log::Info, boost::format("Pobranie segmentu %1% nie powiodlo sie... Przelaczanie skrzynki...") %(mySeg + 1) );
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
			
	return 0;
}

int CLibMoor::startDownload() {
	bool segValid = true;
	int curSeg = mySeg;
	while (segValid && (mySeg < myHash->getInfo().numOfSegments)) {
		++curSeg;
		LOG(Log::Info, boost::format( "Sciaganie segmentu: %1%/%2%" )
		               %curSeg
		               %myHash->getInfo().numOfSegments);
		segValid = false;
		for (int i = 0; i < segDownloadTries && !segValid; ++i) {
			if (myMailBox->downloadRequest(curSeg) == 0) {
				segValid = true;
				mySeg = curSeg;
			}
		}
	}
	
	if (segValid) {
		LOG(Log::Info, "Wszystkie segmenty sciagnieto pomyslnie... Koncze pobieranie.");
		downloadDone = true;
	}
	
	return segValid;
}

Status CLibMoor::getStatus() {
	Status s(mySeg, myMailBox->getSpeed(), myMailBox->getBytesRead(),
	         myHash->getInfo().accounts[selected].name);
	return s;
}

unsigned int CLibMoor::getLastSegment(const std::string& filePath) {
	unsigned int segment = 0;
	if (boost::filesystem::exists(filePath)) {
		unsigned int filesize = boost::filesystem::file_size(filePath);
		if (myHash->getInfo().segmentSize != 0) {
			segment = filesize / myHash->getInfo().segmentSize;
		}
		else {
			int currSize = 0;
			std::vector<int>::const_iterator it = myHash->getInfo().segmentSizes.begin();
			for (; it != myHash->getInfo().segmentSizes.end(); ++it) {
				if ((currSize += *it) < filesize)	break;
				else ++segment;
			}
		}
	}

	return segment;
}
