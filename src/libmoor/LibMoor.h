/*
 * This file is a part of Moorie
 *
 * Copyright (C) 2007 Moorie Team - http://moorie.pl/
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
                bool started;

                Status getDownloadStatus();
                Status getUploadStatus();
                void pauseDownload();
                void unpauseDownload();
                int getMyUploadNumOfSeg();
                std::string getMyUploadFileCRC();

		// upload
                int selectUploadMailBox(std::string login, std::string passwd, std::string adressee, std::string downPasswd, std::string editPasswd);
		int splitFile(std::string filename, int size);
		int startUpload(unsigned int fromseg);
		std::string addMirror(std::string editpass, std::string orighash, std::string mboxaddr, std::string mboxpass); // adding mirror to hashcode
                std::string generateCleanHashcode(); // clear hashcode generator

	private:
		/**
		 * Gets number of last downloaded segment.
		 * @param filePath Path to downloaded file.
		 * @return Last downloaded segment.
		 */
		unsigned int getLastSegment(const std::string& filePath);

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
                std::string myDownPasswd;
                std::string myEditPasswd ;
                std::vector<std::string> address;

};

#endif
