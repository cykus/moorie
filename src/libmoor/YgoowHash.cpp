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

#include "YgoowHash.h"
// #include "Account.h"
//#include "Util.h"
//#include "Log.h"
#include "Decoder.h"
#include "Mailboxes.h"
#include <stdlib.h>
#include <mcrypt.h>
#include <mhash.h>
#include <boost/scoped_array.hpp>
#include <boost/format.hpp>
#include <boost/regex.hpp>
#include <sstream>
#include <iomanip>

using namespace std;

YgoowHash::YgoowHash(): valid(false)
{
}

YgoowHash::YgoowHash(std::string hash)
{
	decode(hash);
}

YgoowHash::~YgoowHash()
{
}

const unsigned char* YgoowHash::getKey()
{
	static const unsigned char keys[][32] = { 
		{ 0x10, 1, 0, 0x51, 0x31, 0xea, 0xc1, 0x2d, 0x63, 0x55, 0x9e, 0xa2, 0x71, 0x31, 0x5f, 0, 0x37, 0x54, 0x95, 6, 50, 50, 50, 0x12, 20, 0x12, 100, 100, 0x51, 0xa9, 0xc5, 0xff },
  		{0, 0}
	};

	for (int i=0; keys[i][0]; i++)
			return &keys[i][0];

//	return NULL;
}

const unsigned char* YgoowHash::getIV()
{
	static const unsigned char ivec[][32] = { 
		{ 0x10, 1, 1, 0x11, 0x31, 20, 0x15, 0xe1, 0x79, 0xa7, 0x92, 20, 0x2a, 0x63, 0x56, 0x85, 0xe4, 50, 180, 170, 0xf4, 0x3a, 0x36, 0x2e, 0x1c, 0xb3, 0xe4, 0x3a, 0, 0x56, 0x15, 0x77 },
  		{0, 0}
	};

	for (int i=0; ivec[i][0]; i++)
			return &ivec[i][0];
//	return NULL;
}

std::string YgoowHash::getMD5( const std::string & str )
{
	MHASH mh = mhash_init( MHASH_MD5 );
	if ( mh != MHASH_FAILED ) {
		boost::scoped_array<unsigned char> md5hash( new unsigned char [ mhash_get_block_size( MHASH_MD5 ) ] );
		mhash( mh, str.c_str(), str.size() );
		mhash_deinit( mh, md5hash.get() );
		std::stringstream ss;
		// TODO get rid of boost::format
		for (unsigned int i = 0; i < mhash_get_block_size( MHASH_MD5 ); i++) {
			ss <<  ( boost::format( "%02x" ) % static_cast<unsigned int>( md5hash[i] ) );
		}
		return ss.str();
	}
//	LOG( Log::Error, "md5 init failed" );
	return std::string(); //TODO
}

std::vector<int> YgoowHash::split(char *str, int len)
{
	const char c = '|';
	std::vector<int> v;
	int begin = 0;
	int i = 0;
	int tokens = 0;
	while (i<len)
	{
		if (str[i] == c)
		{
			v.push_back(begin);
			v.push_back(i - begin);
			begin = i + 1;
			++tokens;
			if (tokens == 1) // special case for crc byte - always 4, may contain 0x7c character
			{
				i += 5;
				++tokens;
				continue;
			}
		}
		++i;
		cout << tokens << endl;
	}
	return v;
}

bool YgoowHash::decode(std::string hash)
{
	string strHash;
	string str2;
	string str3;
	string str4;
	string str5;
	strHash = hash;
	
	MCRYPT td;
	
	/*
	for (int i = 0; i < strHash.length(); i++) {
		if (strHash[i] == '\n')
			strHash.erase(i, i);
		else if (strHash[i] == '\r')
			strHash.erase(i, i);
		else if (strHash[i] == '\t')
			strHash[i] = '\0';
	}

	strHash.erase(0, 9);
	
	string hashin[10];
	int j; j=0;
	for (int i = 0; i < strHash.length(); i++) {
		if (strHash[i] == '|') {
			j++; i++;
		}
		hashin[j] += strHash[i];
	}
	
	fileName = hashin[0]; // strArray[1]
	str5 = hashin[2];
	int declen = str5.length();
	
	string array = Rot13(str5);
 	cout << endl << endl << array<< endl;
	std::reverse(array.begin(), array.end());
	cout << endl << endl << array<< endl;
// 	cout << endl << "Oryginal: " << endl <<  hash << endl;
	unsigned char *in = unbase64(const_cast<char *>(array.c_str()), strlen(array.c_str()));
	cout << in << endl;
	*/
	
	string myhashin[10];
	int j; j=0;
	for (int i = 0; i < strHash.length(); i++) {
		if (strHash[i] == '|') {
			j++; i++;
		}
		myhashin[j] += strHash[i];
	}
	
	hash = myhashin[3]; 
	
	int declen;
	int i;
	int hpos;
	
	
//	cout << endl << endl << array<< endl;
	std::reverse(hash.begin(), hash.end());

//	hash = Rot13(hash);
	cout << hash << endl;
	boost::scoped_array<char> hashin(new char [hash.size() + hash.size()/64 + 1]); //allocate room for reformatted (including newlines) hash string

	//
	// reformat hash code - add new lines, remove or replace some
	// characters
	i = 0;
	for (hpos = 0; hash[hpos]; hpos++)
	{
		if (hash[hpos] == '>' || hash[hpos] == ' ' || hash[hpos] == '\t')
			continue;
		if (hash[hpos] == '-')
			hashin[i++] = '+';
		else
			hashin[i++] = hash[hpos];
		if ((i % 64) == 0)
			hashin[i++] = '\n';
	}
//	cout << i << endl;
	if (i == 0)
		return false; // empty hash code

	hashin[i++] = '\n';
	hashin[i] = 0;
	
	
	hashString = std::string(&hashin[0]);
	cout << hashString << endl;


	//
	// strip off moorhunt prefixes
	unsigned char *in = unbase64(const_cast<char *>(hashString.c_str()), strlen(hashString.c_str()));
	declen = strlen(hashString.c_str());
	cout << in << " " << declen << endl;
	td = mcrypt_module_open((char*)"rijndael-256", NULL, (char*)"ofb", NULL);


	unsigned char *key = const_cast<unsigned char *>(getKey());
	unsigned char *iv = const_cast<unsigned char *>(getIV());

	cout << " KEY: " << key << " IV: " << iv << endl;
	
	if (key == NULL || iv == NULL)
		return 1;

	if (mcrypt_generic_init(td, key, 32, iv) < 0)
	{
		mcrypt_module_close(td);
		return 1;
	}
	
	if (mdecrypt_generic(td, in, declen) != 0)
	{
		mcrypt_module_close(td);
		return 1;
	}
//		LOG_BUFFER(Log::Debug, reinterpret_cast<char *>(in), declen);
	mcrypt_generic_deinit(td);
	mcrypt_module_close(td);
	cout << in << " " << declen << endl;
	
	bool valid = true;
	
	
	return valid;
}

bool YgoowHash::isValid() const
{
	return valid;
}

string YgoowHash::getFileName() const
{
	return fileName;
}

long YgoowHash::getFileSize() const
{
	return fileSize;
}

int YgoowHash::getNumOfSegments() const
{
	return numOfSegments;
}

int YgoowHash::getNumOfMirrors() const
{
	return accounts.size();
}

std::vector<std::string> YgoowHash::getAccounts() const
{
 	return accounts;
}


std::string YgoowHash::getForWhom() const
{
	return forWhom;
}

bool YgoowHash::usesMD5Passwords() const
{
	return ( verMaj == 'a' && ( verMin == 'g' || verMin == 'h' ) );
}

bool YgoowHash::isAccessPasswordProtected() const
{
	return accessPasswd.size() > 0;
}

std::string YgoowHash::getAccessPassword() const
{
	return accessPasswd;
}

bool YgoowHash::checkAccessPassword( const std::string & pass ) const
{
	if ( !isAccessPasswordProtected() ) {
		return true;
	}
	if ( usesMD5Passwords() ) {
		return getMD5( pass ) == accessPasswd;
	}
	return pass == accessPasswd;
}

std::string YgoowHash::getEditPassword() const
{
	return editPasswd;
}

bool YgoowHash::checkEditPassword( const std::string & pass ) const
{
	if ( usesMD5Passwords() ) {
		return getMD5( pass ) == editPasswd;
	}
	return pass == editPasswd;
}

std::string YgoowHash::getCoverURL() const
{
	return coverURL;
}

std::string YgoowHash::getDescriptionURL() const
{
	return descURL;
}

std::string YgoowHash::getFullTitle() const
{
	return fullTitle;
}

std::string YgoowHash::getUploader() const
{
	return uploader;
}

std::string YgoowHash::getComment() const
{
	return comment;
}

unsigned int YgoowHash::getCrc() const
{
	return crc;
}

std::string YgoowHash::getHashString() const
{
	return hashString;
}

bool YgoowHash::operator==(const YgoowHash &m)
{
	if (fileName != m.fileName)
		return false;
	//TODO: check all conditions
	return true;
}

bool YgoowHash::read(std::ifstream &f)
{
        const boost::regex re("^\\s*#");
	std::string hashStr;
	while (f.good())
	{
		std::string line;
		getline(f, line);
		if (boost::regex_search(line, re))
			continue;
		hashStr += line;
		if (line.find(">>") != std::string::npos)
		{
//			LOG(Log::Debug, "Found Ygoow hash");
			decode(hashStr);
			hashStr = "";
			return true;
		}
	}
	return false;
}

std::list<YgoowHash> YgoowHash::fromFile(std::ifstream &f)
{
	std::list<YgoowHash> mh;
	while (f.good())
	{
		YgoowHash h;
		if (h.read(f))
			mh.push_back(h);
	}
	return mh;
}
