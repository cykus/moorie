#ifndef _LIBMOOR_H_
#define _LIBMOOR_H_

#include <string>

#include "Hash.h"
#include "Mailbox.h"
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
		bool uploadDone;
		Status getStatus();
		
		// upload
		int selectUploadMailBox(int mailbox, std::string login, std::string passwd);
		int splitFile(std::string filename, int size);
		int startUpload();
	private:
		/** 
		 * Gets number of last downloaded segment.
		 * @param filePath Path to downloaded file.
		 * @return Last downloaded segment.
		 */
		unsigned int getLastSegment(const std::string& filePath);

		

	private:
		boost::shared_ptr<Hash> myHash;
		CMailBox* myMailBox;
		int mySeg;
		
		// upload
		std::string myUploadFilename;
		int selected;
		int segments;
		std::string myUploadMailbox;
		std::string myLogin;
		std::string myPasswd;
};

#endif
