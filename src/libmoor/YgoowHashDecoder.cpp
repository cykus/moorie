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

#include "YgoowHashDecoder.h"
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

#include "YgoowHash.h"
#include "HashUtils.h"

const unsigned char keys[][32] = {
	{0x10, 1, 0, 0x51, 0x31, 0xea, 0xc1, 0x2d, 0x63, 0x55, 0x9e, 0xa2, 0x71, 0x31, 0x5f, 0, 0x37, 0x54, 0x95, 6, 50, 50, 50, 0x12, 20, 0x12, 100, 100, 0x51, 0xa9, 0xc5, 0xff},
	{0, 0}
};

const unsigned char ivec[][32] = {
	{0x10, 1, 1, 0x11, 0x31, 20, 0x15, 0xe1, 0x79, 0xa7, 0x92, 20, 0x2a, 0x63, 0x56, 0x85, 0xe4, 50, 180, 170, 0xf4, 0x3a, 0x36, 0x2e, 0x1c, 0xb3, 0xe4, 0x3a, 0, 0x56, 0x15, 0x77},
	{0, 0}
};

Hash* YgoowHashDecoder::decode(const std::string& hashcode) {
	HashInfo result;
	string strHash;
	string str2;
	string str3;
	string str4;
	string str5;
	strHash = hashcode;
	
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
	for (int i = 0; i < hashcode.length(); i++) {
		if (hashcode[i] == '|') {
			j++; i++;
		}
		myhashin[j] += hashcode[i];
	}
	
	strHash = myhashin[3];
 	strHash = Rot13(strHash);
	
	int declen;
	int i;
	int hpos;
	
	
//	cout << endl << endl << array<< endl;
	std::reverse(strHash.begin(), strHash.end());

// 	hash = Rot13(hash);
	std::cout << strHash << std::endl;
	boost::scoped_array<char> hashin(new char [strHash.size() + strHash.size()/64 + 1]); //allocate room for reformatted (including newlines) hash string

	//
	// reformat hash code - add new lines, remove or replace some
	// characters
	i = 0;
	for (hpos = 0; strHash[hpos]; hpos++)
	{
		if (strHash[hpos] == '>' || strHash[hpos] == ' ' || strHash[hpos] == '\t')
			continue;
		if (strHash[hpos] == '-')
			hashin[i++] = '+';
		else
			hashin[i++] = strHash[hpos];
 		if ((i % 64) == 0)
 			hashin[i++] = '\n';
	}
//	cout << i << endl;
	if (i == 0)
		return new YgoowHash(result); // empty hash code

	hashin[i++] = '\n';
	hashin[i] = 0;
	
	
	result.hashString = std::string(&hashin[0]);
	std::cout << result.hashString << std::endl;


	//
	// strip off moorhunt prefixes
	unsigned char *in = unbase64(const_cast<char *>(result.hashString.c_str()), strlen(result.hashString.c_str()));
	declen = strlen(result.hashString.c_str());
	std::cout << in << " " << declen << std::endl;
	td = mcrypt_module_open((char*)"rijndael-256", NULL, (char*)"cbc", NULL);


	unsigned char *key = const_cast<unsigned char*>(&keys[0][0]);
	unsigned char *iv = const_cast<unsigned char*>(&ivec[0][0]);

	std::cout << " KEY: " << key << " IV: " << iv << std::endl;
	
	
	if (key == NULL || iv == NULL)
		return new YgoowHash(result);

	if (mcrypt_generic_init(td, key, 32, iv) < 0)
	{
		mcrypt_module_close(td);
		return new YgoowHash(result);
	}
	
	if (mdecrypt_generic(td, in, declen) != 0)
	{
		mcrypt_module_close(td);
		return new YgoowHash(result);
	}
//		LOG_BUFFER(Log::Debug, reinterpret_cast<char *>(in), declen);
	mcrypt_generic_deinit(td);
	mcrypt_module_close(td);
	std::cout << in << " " << declen << std::endl;
	
	result.valid = true;
	
	return new YgoowHash(result);
}

// bool YgoowHash::usesMD5Passwords() const
// {
// 	return ( verMaj == 'a' && ( verMin == 'g' || verMin == 'h' ) );
// }

// bool YgoowHash::checkAccessPassword( const std::string & pass ) const
// {
// 	if ( !isAccessPasswordProtected() ) {
// 		return true;
// 	}
// 	if ( usesMD5Passwords() ) {
// 		return getMD5( pass ) == accessPasswd;
// 	}
// 	return pass == accessPasswd;
// }

// bool YgoowHash::checkEditPassword( const std::string & pass ) const
// {
// 	if ( usesMD5Passwords() ) {
// 		return getMD5( pass ) == editPasswd;
// 	}
// 	return pass == editPasswd;
// }


// bool YgoowHash::read(std::ifstream &f)
// {
//         const boost::regex re("^\\s*#");
// 	std::string hashStr;
// 	while (f.good())
// 	{
// 		std::string line;
// 		getline(f, line);
// 		if (boost::regex_search(line, re))
// 			continue;
// 		hashStr += line;
// 		if (line.find(">>") != std::string::npos)
// 		{
// //			LOG(Log::Debug, "Found Ygoow hash");
// 			decode(hashStr);
// 			hashStr = "";
// 			return true;
// 		}
// 	}
// 	return false;
// }

// std::list<YgoowHash> YgoowHash::fromFile(std::ifstream &f)
// {
// 	std::list<YgoowHash> mh;
// 	while (f.good())
// 	{
// 		YgoowHash h;
// 		if (h.read(f))
// 			mh.push_back(h);
// 	}
// 	return mh;
// }
