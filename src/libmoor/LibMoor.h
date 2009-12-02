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
		int selectMailBox(int MailBox, std::string path = "");
		int startDownload();
		Status getStatus();

	public:
		bool downloadDone;

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
		int selected;
};

#endif
