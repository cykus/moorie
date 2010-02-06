/*
 * This file is a part of Moorie.
 *
 * Copyright (C) 2007 Pawel Stolowski <pawel.stolowski@wp.pl>
 *
 * Moorie is free software; you can redestribute it and/or modify it
 * under terms of GNU General Public License by Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY. See GPL for more details.
 */

#ifndef MOORHUNTHASHDECODER_H__
#define MOORHUNTHASHDECODER_H__

#include <string>

#include "HashDecoder.h"
#include "Tools.h"


class MoorhuntHashDecoder : public HashDecoder {
	public:
		~MoorhuntHashDecoder() {}

	public:
		virtual Hash* decode(const std::string& hashcode);
};

class MoorhuntHashEncoder {
	    std::string myFilename;
	    std::string myCRC;
	    int myFileSize;
	    bool myRound;
	    bool myRandName;
	    int mySegmentCount;
	    int myMinSize;
	    int myMaxSize;
	    const char *myDownloadPassword;
	    int myMirrors;
	    // tutaj lista mirrorow
	    std::string myAvartLink;
	    const char *myEditPassword;
	    std::string myFor;
	    std::string myFullDescriptionLink;
	    std::string myFullTitle;
	    std::string myUploader;
	    std::string myComment;
		std::vector<std::string> mailboxes;

	public:
		MoorhuntHashEncoder();
		~MoorhuntHashEncoder();
		void setFilename(std::string data) { myFilename = data; };
		void setCRC(std::string data) { myCRC = data; };
		void setFileSize(int data) { myFileSize = data; };
		void setRound(bool data) { myRound = data; };
		void setRandName(bool data) { myRandName = data; };
		void setSegmentCount(int data) { mySegmentCount = data; };
		void setSegSize(int data) { myMinSize = data; myMaxSize = data; };
		void setDownloadPassword(std::string data) { myDownloadPassword = data.c_str(); };
		void setMirrors(int data) { myMirrors = data; };
		void setAvartLink(std::string data) { myAvartLink = data; };
		void setEditPassword(std::string data) { myEditPassword = data.c_str(); };

		const bool decode(std::string hashcode);
		const std::string encode();

		const std::string addNewMirror(std::string editpassword, std::string hashcode, std::string mailbox, std::string password);
};


#endif // MOORHUNTHASHDECODER_H__

