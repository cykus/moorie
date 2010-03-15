/*
 * This file is a part of Moorie
 *
 * Copyright (C) 2007-2010 Moorie Team - http://moorie.pl/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "LibMoor.h"

#include <iostream>

#include "Log.h"
#include "MailboxFactory.h"
#include "HashManager.h"
#include "MoorhuntHashDecoder.h"

const int segDownloadTries = 2;

CLibMoor::CLibMoor()
	: mySeg(0),
		selected(0),
		downloadDone(false),
                started(false),
                downloadPaused(false)
{
}

CLibMoor::~CLibMoor() { }

bool CLibMoor::Dehash(const std::string& hashcode) {
	myHash = boost::shared_ptr<Hash>(HashManager::fromString(hashcode));
	if (myHash->getInfo().valid) {
		int vector_size = myHash->getInfo().accounts.size();
		for (int i = 0; i < vector_size; ++i) {
			std::cout << i+1 << " " << myHash->getInfo().accounts[i].name << std::endl;
		}
	}

	return myHash->getInfo().valid;
}

int CLibMoor::selectDownloadMailBox(int MailBox, std::string path) {

	myPath = path;

        LOG(Log::Info, boost::format("Pobieranie do %1%") %path);
        mySeg = getLastSegment(path + myHash->getInfo().fileName);
        if (mySeg == myHash->getInfo().numOfSegments) {
            LOG(Log::Info, "Plik pobrano w calosci, przerywam...");
            return 1;
        }

	int vector_size = myHash->getInfo().accounts.size();

	if (MailBox > vector_size -1) {
		selected = 0;
		LOG(Log::Info ,"Skrzynka nie istnieje, wybieram pierwsza z listy");
	}
	else
		selected = MailBox;
	/*for (int i = 0; i < vector_size; ++i) {
		LOG(Log::Debug, boost::format( "%1%. ID: %2% L: %3% P: %4%" ) %i
										%myHash->getInfo().accounts[i].name
										%myHash->getInfo().accounts[i].login
										%myHash->getInfo().accounts[i].password);
	}*/

	int tries = 0;
	int baddownloads = 0;
	while (tries <= myHash->getInfo().accounts.size() || baddownloads <= (myHash->getInfo().accounts.size()*2) && downloadDone == false ) {
		std::string mailbox = myHash->getInfo().accounts[selected].name;
		std::string login = myHash->getInfo().accounts[selected].login;
		std::string passwd = myHash->getInfo().accounts[selected].password;

//		validMailbox = true;

                myMailBox = MailboxFactory::Instance().Create(mailbox, login, passwd);
		if (myMailBox) {
                        started = true;
			LOG(Log::Info, boost::format( "Logowanie do:  %1%" )
			               %myHash->getInfo().accounts[selected].name);
                        state = Status::Connecting;
			if (myMailBox->loginRequest() == 0) {
				tries = 0;
				myMailBox->setFileName(path + myHash->getInfo().fileName);
				myMailBox->setFileCRC(myHash->getInfo().crc);
                                state = Status::Connected;
				LOG(Log::Info, "Zalogowano pomyslnie...");
				LOG(Log::Info, "Sprawdzanie listy segmentow...");
				myMailBox->getHeadersRequest();
				unsigned int segments = myMailBox->countAvailableSegments(mySeg);
				bool found;
				if (segments == 0) {
					LOG(Log::Info, "--== Nie znaleziono zadnego segmentu! Zmieniam skrzynke ==--");
					found = false;
				} else {
					int tmpseg;
					bool missing;
					for (int i = 1; i <= myHash->getInfo().numOfSegments; i++) {
						tmpseg = myMailBox->checkAvailableSegment(i);
						if (tmpseg != true) {
							LOG(Log::Info,  boost::format("--== Uwaga, brakuje segmentu: %1% ==--") %(i));
							missing = true;
						}
					}
					if (missing != true) {
						LOG(Log::Info, "Znaleziono wszystkie segmenty, rozpoczynam pobieranie...");
					}
					else {
						LOG(Log::Info, "Na skrzynce brakuje segmentu(ow), pobieranie moze sie nie udac");
						// TODO - pytanie o zmiane skrzynki?
					}
					found = true;
				}
				if (found == true) {
					state = Status::Downloading;
					if (startDownload() == 0) {
						LOG(Log::Info, boost::format("Pobranie segmentu %1% nie powiodlo sie... Przelaczanie skrzynki...") %(mySeg + 1) );
						state = Status::SegmentError;
						baddownloads++;
					}
				}

			}
			else {
				LOG(Log::Info, "Logowanie nie powiodlo sie..." );
                                state = Status::ConnectionError;
			}
		}
		// Program should never reach this execution path, if decoder is
		// implemented properly!
		else {
			LOG(Log::Info, "Wybrana skrzynka nie jest obsługiwana w tej wersji programu.");
		}

		if (downloadDone == true) {
			myMailBox->setFileCRC(myHash->getInfo().crc);
			std::string crcFromHash = myMailBox->getFileCRC();
			LOG(Log::Info, boost::format ("Sprawdzanie CRC sciagnietego pliku, oczekiwane CRC: [%1%]") %crcFromHash);
			myMailBox->calculateFileCRC(myPath + myHash->getInfo().fileName);
			std::string fileCRC = myMailBox->getFileCRC();
			LOG(Log::Info, boost::format ("CRC sciagnietego pliku: [%1%]") %fileCRC);

			if (fileCRC.compare(crcFromHash) != 0){
				LOG(Log::Error, "-- Zle CRC sciagnietego pliku! --");
				state = Status::FileError;
			} else {
				LOG(Log::Info, "-- CRC OK! --");
				state = Status::Finished;
			}
			break;
		}

		if (tries >= myHash->getInfo().accounts.size()) {
			LOG(Log::Info, "Nie udalo sie pobrac pliku z zadnej ze skrzynek... Koncze program." );
			state = Status::FileError;
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

	if (segValid && curSeg >= myHash->getInfo().numOfSegments) {

		LOG(Log::Info, "Wszystkie segmenty sciagnieto pomyslnie... Koncze pobieranie.");
		downloadDone = true;
		state = Status::Downloaded;
	}
	return segValid;
}

int CLibMoor::splitFile(std::string filename, int size) {
        myUploadFilename = filename;
        myUploadFilesize = 0;
        myUploadSegSize = size*1024*1024;

        LOG(Log::Info, boost::format("Dzielenie pliku %1% na segmenty") % filename);
        int mysegsize = size*1024*1024;
        int bytes = 0; int read = 0;
        segments = 1;
        char buffer[128*1024];
        std::stringstream ss;
        ss << filename << "." << segments;
        std::cout << ss.str() << std::endl;

        std::ifstream in(filename.c_str(), std::ifstream::binary);
        std::ofstream *out = new std::ofstream(ss.str().c_str(), std::ios::out | std::ofstream::binary);
        LOG(Log::Debug, boost::format( "Seg: %1%" )	%segments);

        while (!in.eof()) {
                in.read(buffer, 128*1024);
                read = in.gcount();
                out->write(buffer, read);
                bytes += read;
                myUploadFilesize+=read;
// 		std::cout << "Read: " << read << std::endl;

                if (bytes == mysegsize) {
                        bytes = 0;
                        segments++;
                        delete out;
                        ss.str("");
                        ss << filename << "." << segments;
                        out = new std::ofstream(ss.str().c_str(), std::ios::out | std::ofstream::binary);
                        LOG(Log::Debug, boost::format( "Seg: %1%" )	%segments);
                }
        }
        myUploadNumOfSeg = segments;
        return 0;

}
int CLibMoor::selectUploadMailBox(std::string login, std::string passwd, std::string adressee, std::string downPasswd, std::string editPasswd) {

// 	myUploadMailbox = getMailboxName(id);
        boost::regex mail_rgx("^(.*)@(.*)");
        boost::smatch result_sth;
        boost::regex_match(login, result_sth, mail_rgx);

        downloadDone = false;
        myLogin = result_sth[1];
        myPasswd = passwd;
        myDownPasswd = downPasswd;
        myEditPasswd = editPasswd;
	int adr_size = adressee.size();
	int i2=0;
	for (int i = 0; i<adr_size; ++i)
	{
		if (adressee.at(i) == ',')
		{
			address.push_back(adressee.substr(i2, i));
			i2=i+1;
		}
		else if(i == (adr_size-1))
			address.push_back(adressee.substr(i2,i+1));
	}
        myUploadMailbox = result_sth[2]; // TODO - wybieranie skrzynki po id;

        std::string upload_mailbox = myLogin+"@"+myUploadMailbox;

        myMailBox = MailboxFactory::Instance().Create(myUploadMailbox, myLogin, myPasswd);
        if (myMailBox){
            started = true;
            LOG(Log::Info, boost::format( "Logowanie do:  %1%" ) %upload_mailbox);
// 		LOG(Log::Info, boost::format( "Logowanie do: ...") );
            state = Status::Connecting;
            if (myMailBox->loginRequest() == 0) {
                    LOG(Log::Info, boost::format( "Zalogowano pomyslnie!" ));
                    state = Status::Connected;

                    myMailBox->calculateFileCRC(myUploadFilename);
                    myUploadFileCRC = myMailBox->getFileCRC();
                    LOG(Log::Debug, boost::format( "CRC Pliku: %1%" ) %myMailBox->getFileCRC());
            }
            else {
                    LOG(Log::Info, boost::format( "Logowanie nie powiodlo sie, przerywam." ));
                    downloadDone = true;
                    state = Status::FileError;
                    return 1;
            }
            return 0;
        }
        else {
            LOG(Log::Info, "Wybrana skrzynka nie jest obsługiwana w tej wersji programu.");
            return 1;
        }
}
int CLibMoor::startUpload(unsigned int fromseg) {
	LOG(Log::Info, boost::format("Zaczynam upload od segmentu: %1%") %fromseg);

        std::stringstream ss;
        for (mySeg = fromseg; mySeg <= segments; mySeg++) {
            state = Status::Uploading;
            ss.str("");

            ss << myUploadFilename << "." << mySeg;

            if (myMailBox->uploadRequest(ss.str(), address, mySeg) == 0){
                LOG(Log::Info, boost::format( "Segment %1% wrzucony" )	%mySeg);
                state = Status::Uploaded;
            }
            else {
                LOG(Log::Error, boost::format( "Nie udalo sie wrzucic segmentu nr %1% " )	%mySeg);
                state = Status::SegmentError;
            }
        }
        LOG(Log::Info, boost::format( "Upload zakonczony!" ));
        downloadDone = true;
        state = Status::FinishedUpload;
	return 0;
}

std::string CLibMoor::generateCleanHashcode() {
	MoorhuntHashEncoder *hashEncoder;
	hashEncoder = new MoorhuntHashEncoder();


        std::string downpass = myDownPasswd;
        std::string editpass = myEditPasswd;

	hashEncoder->setFilename(myUploadFilename);
	hashEncoder->setCRC(myUploadFileCRC);
	hashEncoder->setFileSize(myUploadFilesize);
	hashEncoder->setSegmentCount(myUploadNumOfSeg);
	hashEncoder->setSegSize(myUploadSegSize);
        hashEncoder->setDownloadPassword(downpass);
 	hashEncoder->setMirrors(0); // na razie 0 mirrorow
        hashEncoder->setEditPassword(editpass);

	std::string hash = hashEncoder->encodeClean();

	delete hashEncoder;
// 	std::cout << "HASH: " << hash << std::endl;
	return hash;
}

std::string CLibMoor::addMirror(std::string editpass, std::string orighash, std::string mboxaddr, std::string mboxpass) {
 	MoorhuntHashEncoder *encoder;
        encoder = new MoorhuntHashEncoder();

	std::string newhash = encoder->addNewMirror(editpass, orighash, mboxaddr, mboxpass);
        delete encoder;
	return newhash;
}

Status CLibMoor::getDownloadStatus() {
        Status s(mySeg, myMailBox->getDownloadSpeed(), myMailBox->getBytesRead(),
                 myHash->getInfo().accounts[selected].name, state);
	return s;
}
Status CLibMoor::getUploadStatus() {
        Status s(mySeg, myMailBox->getUploadSpeed(), myMailBox->getBytesSend(),
                 myUploadMailbox, state);
        return s;
}
void CLibMoor::pauseDownload() {
        if(myMailBox->pauseDownload()) downloadPaused = true;
}
void CLibMoor::unpauseDownload() {
        if(myMailBox->unpauseDownload()) downloadPaused = false;
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
int CLibMoor::getMyUploadNumOfSeg(){
    return myUploadNumOfSeg;
}
std::string CLibMoor::getMyUploadFileCRC(){
    return myUploadFileCRC;
}

/// writer function for checkVersion()
size_t CLibMoor::writer(char *data, size_t size, size_t nmemb, std::string *buffer)
{
  int result = 0;
  if (buffer != NULL)
  {
      buffer->append(data, size * nmemb);
      result = size * nmemb;
  }
  return result;
}

std::string CLibMoor::checkVersion(std::string myver) {
	std::string buffer;

	CURL *curl;
	CURLcode result;

	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://up.moorie.pl/ver"  );
		curl_easy_setopt(curl, CURLOPT_HEADER, 0);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CLibMoor::writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		result = curl_easy_perform(curl);//http get performed
		curl_easy_cleanup(curl);//must cleanup

		myServerVersion = buffer;

 		if (result == CURLE_OK) {
			if (myver.compare(buffer) != 0) {
				return buffer; // new version avalilable :)
			} else
				return ""; // no new version :(
		}
		else
			return "";
	}
	return "";
}
