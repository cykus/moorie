#ifndef _LIBMOOR_H_
#define _LIBMOOR_H_

#include <string>

#include "Hash.h"
#include "Mailbox.h"
#include "Mailboxes.h"
#include "Status.h"

class CLibMoor {
	public:
		CLibMoor();
		~CLibMoor();

	public:
		bool Dehash(const std::string& hashcode);
		int selectDownloadMailBox(int MailBox, std::string path = "");
		int startDownload();
		bool downloadDone;
                bool downloadPaused;
                bool uploadDone;

		Status getStatus();
                void pauseDownload();
                void unpauseDownload();

		// upload
        int selectUploadMailBox(std::string login, std::string passwd, std::string adressee);
		int splitFile(std::string filename, int size);
		int startUpload(unsigned int fromseg);
	private:
		/**
		 * Gets number of last downloaded segment.
		 * @param filePath Path to downloaded file.
		 * @return Last downloaded segment.
		 */
		unsigned int getLastSegment(const std::string& filePath);

		std::string generateCleanHashcode(); // clear hashcode generator

		boost::shared_ptr<Hash> myHash;
		CMailBox* myMailBox;
		int mySeg;
		std::string myPath;
                Status::State state;

                // upload
		std::string myUploadFilename;
		int myUploadFilesize;
		int myUploadSegSize;
		std::string myUploadAccessPasswd;
		std::string myUploadEditPasswd;
		int myUploadNumOfSeg;
		std::string myUploadFileCRC;

		int selected;
		int segments;
		std::string myUploadMailbox;
		std::string myLogin;
		std::string myPasswd;
		std::vector<std::string> address;

};

#endif
