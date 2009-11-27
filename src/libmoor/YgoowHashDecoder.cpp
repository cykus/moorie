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
#include <boost/scoped_array.hpp>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "YgoowHash.h"
#include "HashUtils.h"
#include "Tools.h"
#include "Decoder.h"
#include "StringUtils.h"
#include "Utils.h"
#include "BinaryStream.h"

const char* needles[] = {"~", "{", "}", "=", "<", "?", "[YKP]:", 0};
const char* replacements[] = {"*", "-", ".", "0", "1", "2", "[KontoPomocnicze]:", 0};

Hash* YgoowHashDecoder::decode(const std::string& hashcode) {
	HashInfo result;
	std::string hash = hashcode;

	hash = strReplace(hash, "\r\n", "");
	hash = strReplace(hash, "\n", "");
	hash = strReplace(hash, "ygoow://", "Ygoow://");
	
	std::string str = hash.substr(strReplace(hash, "Ygoow://|", "").find('|') + 9);
	str = strReplace(str, " ", "");
	str = strReplace(str, "-", "+");
	hash = hash.substr(0, strReplace(hash, "Ygoow://|", "").find('|') + 9) + str;

	std::vector<std::string> hashArray = strSplit(hash, '|');

	char ch = 'a';

	std::string str2;
	std::string str3;
	std::string str4;
	std::string str5;
	if (hashArray.size() == 7) {
		str2 = hashArray.at(2);
		str3 = hashArray.at(3);
		str4 = hashArray.at(4);
		str5 = strReplace(hashArray.at(5), "-", "+");
	}
	else {
		str5 = strReplace(hashArray.at(3), "-", "+");
		str5 = strReplace(str5, "HStAAZQAAzg79/4nj7GSaF", "==");
		str5 = strReplace(str5, "58aDDAAChGASu=gaQsfAI9", "=");
		str2 = hashArray.at(2).substr(0, 10);
		str3 = hashArray.at(2).substr(11, 2);
		str4 = hashArray.at(2).substr(13);
		ch = hashArray.at(2).substr(10, 1)[0];
	}
	result.fileName = hashArray.at(1);
	std::cerr << "FileName = " << result.fileName << std::endl;
	
	int num = 0;
	for (int i = 1; i <= 9; ++i) {
		std::string str6 = sha1(i + '0'); // reading char of this int
		if ((str6.substr(0, 10) == str2) && (str6.substr(30, 10) == rot13(str4)))
			num = i;
	}

	int num4 = 0x18 - (strToInt(str3) + num);
	std::string str5_r = rot13(str5);
	std::string reverted(str5_r.rbegin(), str5_r.rend());

	int km = (num % 0x20) * num4;
	if (km == 0) km = 1;
	int im = (num % 10) / num;
	if (im == 0) im = 1;
	
	unsigned char key[] = {
		(im * 2), (0xff - ((km * km) / 2)), (0x4b + ((km * km) / 2)),
		(((30 - km) + 12) * 2), (km * 2), (0x84 + (km * 2)), (0xe4 / km),
		((0x79 - km) + (0x20 / km)), ((0x10 + (km * 0x12)) + 5), (km * 2),
		(0xff - ((km * km) / 2)), (0x4b + ((km * km) / 2)), (((30 - km) + 12) * 2),
		(km * 2), (0x84 + (km * 2)), (0xcf / im), ((0x79 - im) + (0x20 / km)),
		((0x10 + (km * 0x12)) + 5), (0xff - ((km * km) / 2)),
		(0x4b + ((km * km) / 2)), 0x76, (((0x1c - km) + 14) * 2), (km * 2),
		(0x84 + (km * 2)), 0x4a, (220 / km), ((0x65 - km) + (0x20 / km)),
		((0x10 + (km * 0x12)) + 5), (0xd6 - ((km * km) / 2)),
		(0x4b + ((km * km) / 2)), (((0x22 - km) + 11) * 2), (im * 2)
	};
	unsigned char iv[] = {
		(km * 2), (0xf5 - ((im * im) / 2)), (0x4b + ((im * im) / 2)),
		(((30 - im) + 12) * 2), (im * 2), (0x7a + (im * 2)), (0xe4 / im),
		((0x6f - im) + (0x20 / im)), ((0x10 + (im * 0x12)) + 5), (im * 2),
		(0xff - ((im * im) / 2)), (0x36 + ((im * im) / 2)), (((30 - im) + 12) * 2),
		(im * 2), (0x84 + im), (0xd0 / im), ((0x65 - im) + (0x20 / im)),
		((11 + (im * 0x12)) + 3), (im * -2), (0xaf - ((im * im) / 2)),
		(0x4b + ((im * im) / 2)), (((30 - km) + 12) * 2), 80, (0xa2 + (im * 2)),
		(0xda / im), ((0x65 - im) + (0x20 / im)), ((0x10 + (im * 0x12)) + 5),
		(im * 3), (0xe1 - ((im * im) / 2)), (0x4b + ((im * im) / 2)),
		(((0x22 - im) + 12) * 2), km
	};

	unsigned int data_size = 0;
	unsigned char* data = base64_decode(reverted.c_str(), reverted.size(), &data_size);
	decrypt(&data, data_size, key, iv);
	
	// taking ownership of 'data' buffer!
	BinaryStream stream(data, data_size, true);
	result.crc = stream.readUInt32();
	result.fileSize = stream.readInt64();
	result.numOfSegments = stream.readUInt16();
	result.segmentSize = stream.readInt32();
	std::cerr << "CRC = " << result.crc << std::endl;
	std::cerr << "FileSize = " << result.fileSize << std::endl;
	std::cerr << "NumOfSegments = " << result.numOfSegments << std::endl;
	std::cerr << "SegmentSize = " << result.segmentSize << std::endl;
	// Passwords are encrypted as fallows: first SHA1 sum is generated then all
	// its bytes are reversed and finally MD5 is calculated.
	boost::scoped_array<unsigned char> d_passwd(stream.readBytes(16));
	result.accessPasswd = hashToStr(d_passwd.get());
	std::cerr << "DownloadPassword = " << result.accessPasswd << std::endl;
	boost::scoped_array<unsigned char> e_passwd(stream.readBytes(16));
	result.editPasswd = hashToStr(e_passwd.get());
	std::cerr << "EditPassword = "<< result.editPasswd << std::endl;

	/* MAIL BOXES */
	int mboxes_size = stream.readInt32();
	boost::scoped_array<unsigned char> mboxes_data(stream.readBytes(mboxes_size));
	std::string mboxes_raw(mboxes_data.get(), (mboxes_data.get() + mboxes_size));

	// Drawing.Parse
	std::string mboxes_rev(mboxes_raw.rbegin(), mboxes_raw.rend());
	std::string mboxes = "";
	for (size_t i = 0; i < mboxes_rev.size(); ++i)
		mboxes += mboxes_rev[i] + 50;
	mboxes = strReplace(mboxes, needles, replacements);
	// End: Drawing.Parse

	std::vector<std::string> mboxes_sp = strSplit(mboxes, '|');
	for (size_t i = 0; i < mboxes_sp.size(); i+=2) {
		std::vector<std::string> mbox = strSplit(mboxes_sp.at(i), '@');
		std::string server = mbox.at(1);
		std::string login = strReplace(mbox.at(0), "*", "|");
		std::string password = mboxes_sp.at(i+1);
		std::cerr << "SERVER = " << server << ", LOGIN = " << login
		          << ", PASSWD = " << password << std::endl;
	}
	// TODO: sort mboxes based on login containing [KontoPomocnicze]: literal.
	
	/* SIZES */
	int sizes = stream.readInt32();
	for (unsigned int i = 0; i < result.numOfSegments && (i * 3) < sizes; ++i) {
		int size = (stream.readByte() | stream.readByte() << 8 |
		            stream.readByte() << 16 | 0 << 24);
		std::cerr << std::dec << "Size of chunk #" << i + 1 << " = " << size << std::endl;
	}

	/* ADDITINAL DATA */
	std::vector<std::string> adds = strSplit(stream.readString(), '|');
	std::cerr << "For = " << adds.at(0) << std::endl;
	std::cerr << "Uploader = " << adds.at(1) << std::endl;
	std::cerr << "Comment = " << adds.at(2) << std::endl;

	return new YgoowHash(result);
}
