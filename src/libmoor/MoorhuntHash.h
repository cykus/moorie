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

#ifndef __MOORHUNTHASH_H
#define __MOORHUNTHASH_H

#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>

//#include <MailSystem/Account.h>

//! Moorhunt hashfile data
class MoorhuntHash
{
	private:
		bool valid; //!< flag indicating whether hash is valid, set on decoding
		char verMaj;
		char verMin;
		std::string fileName; //!< downloaded file name (as provided in hashfile)
		long fileSize;
		unsigned int crc;
		int numOfSegments;
		int segmentSize;
		std::vector<std::string> accounts;
		std::vector<int> ids;
		std::string forWhom;
		std::string accessPasswd;
		std::string editPasswd;
		std::string coverURL;
		std::string descURL;
		std::string fullTitle;
		std::string uploader;
		std::string comment;
		std::string hashString; //!< raw hash std::string (base64 encoded)

	protected:
		static const unsigned char* getKey(unsigned char vermaj, unsigned char vermin);
		static const unsigned char* getIV(unsigned char vermaj, unsigned char vermin);
		static std::vector<int> split(char *str, int len);
		static std::string getMD5( const std::string & str );
		bool usesMD5Passwords() const;
//		static size_t _writeData(void *buffer, size_t size, size_t nmem, void *ptr);
//		virtual size_t writeData(void *buffer, size_t size, size_t nmem);
		
	public:
		MoorhuntHash();
		MoorhuntHash(std::string hash);
		~MoorhuntHash();

		bool read(std::ifstream &f);
		static std::list<MoorhuntHash> fromFile(std::ifstream &f);
		bool decode(std::string hash);
		bool isValid() const;
		std::string getFileName() const;
		long getFileSize() const;
		int getNumOfSegments() const;
		int getNumOfMirrors() const;
		int getSegmentSize() const;
 		std::vector<std::string> getAccounts() const;
		bool isAccessPasswordProtected() const;
		std::string getForWhom() const;
		std::string getAccessPassword() const;
		bool checkAccessPassword( const std::string & pass ) const;
		std::string getEditPassword() const;
		bool checkEditPassword( const std::string & pass ) const;
		std::string getCoverURL() const;
		std::string getDescriptionURL() const;
		std::string getFullTitle() const;
		std::string getUploader() const;
		std::string getComment() const;
		std::string getHashString() const;
		unsigned int getCrc() const;

		bool operator==(const MoorhuntHash &);
};

#endif

