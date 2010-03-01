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

#include "YgoowHashDecoder.h"

#include <boost/scoped_array.hpp>
#include <iostream>

#include "YgoowHash.h"
#include "HashUtils.h"
#include "Tools.h"
#include "Decoder.h"
#include "StringUtils.h"
#include "BinaryStream.h"
#include "MailboxFactory.h"

const char* needles[] = {"~", "{", "}", "=", "<", "?", "[YKP]:", 0};
const char* replacements[] = {"*", "-", ".", "0", "1", "2", "[KontoPomocnicze]:", 0};

Hash* YgoowHashDecoder::decode(const std::string& hashcode) {
        HashInfo result;
	std::string hash = hashcode;

	hash = str::replace(hash, "\r\n", "");
	hash = str::replace(hash, "\n", "");
	hash = str::replace(hash, "ygoow://", "Ygoow://");
	
	std::string str = hash.substr(str::replace(hash, "Ygoow://|", "").find('|') + 9);
	str = str::replace(str, " ", "");
	str = str::replace(str, "-", "+");
	hash = hash.substr(0, str::replace(hash, "Ygoow://|", "").find('|') + 9) + str;

	result.hashString = hash;
	std::vector<std::string> hashArray = str::split(hash, '|');

	char ch = 'a';

	std::string str2;
	std::string str3;
	std::string str4;
	std::string str5;
	if (hashArray.size() == 7) {
		str2 = hashArray.at(2);
		str3 = hashArray.at(3);
		str4 = hashArray.at(4);
		str5 = str::replace(hashArray.at(5), "-", "+");
	}
	else {
		str5 = str::replace(hashArray.at(3), "-", "+");
		str5 = str::replace(str5, "HStAAZQAAzg79/4nj7GSaF", "==");
		str5 = str::replace(str5, "58aDDAAChGASu=gaQsfAI9", "=");
		str2 = hashArray.at(2).substr(0, 10);
		str3 = hashArray.at(2).substr(11, 2);
		str4 = hashArray.at(2).substr(13);
		ch = hashArray.at(2).substr(10, 1)[0];
	}
	result.fileName = hashArray.at(1);

	int num = 0;
	for (int i = 1; i <= 9; ++i) {
		// change int value into ascii representation
		std::string str6 = sha1(i + '0');
		if ((str6.substr(0, 10) == str2) && (str6.substr(30, 10) == rot13(str4)))
			num = i;
        }

	int num4 = 0x18 - (str::toInt(str3) + num);
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
	// Take ownership of 'data' buffer!
	BinaryStream stream(data, data_size, true);

	if (ch == 'c') {
		result.crc = stream.readUInt32();
		result.fileSize = stream.readInt64();
		result.numOfSegments = stream.readUInt16();
		result.segmentSize = stream.readInt32();
		// Passwords are encrypted as follows: first SHA1 sum is generated then all
		// its bytes are reversed and finally MD5 is calculated.
		boost::scoped_array<unsigned char> d_passwd(stream.readBytes(16));
		result.accessPasswd = hashToStr(d_passwd.get());
		boost::scoped_array<unsigned char> e_passwd(stream.readBytes(16));
		result.editPasswd = hashToStr(e_passwd.get());

		/* MAIL BOXES */
		int mboxes_size = stream.readInt32();
		boost::scoped_array<unsigned char> mboxes_data(stream.readBytes(mboxes_size));
		std::string mboxes_raw(mboxes_data.get(), (mboxes_data.get() + mboxes_size));

		// Drawing.Parse
		std::string mboxes_rev(mboxes_raw.rbegin(), mboxes_raw.rend());
		std::string mboxes = "";
		for (size_t i = 0; i < mboxes_rev.size(); ++i)
			mboxes += mboxes_rev[i] + 50;
		mboxes = str::replace(mboxes, needles, replacements);
		// End: Drawing.Parse

		std::vector<std::string> mboxes_sp = str::split(mboxes, '|');
		for (size_t i = 0; i < mboxes_sp.size(); i+=2) {
			std::vector<std::string> mbox = str::split(mboxes_sp.at(i), '@');
			// Omit unsupported mailboxes!
			if (MailboxFactory::Instance().Registered(mbox.at(1))) {
				HashInfo::MboxAccount account;
				account.name = mbox.at(1);
				account.login = str::replace(mbox.at(0), "*", "|");
				account.password = mboxes_sp.at(i+1);
				result.accounts.push_back(account);
			}
		}
		// TODO: sort mboxes based on login containing [KontoPomocnicze]: literal.

		/* SIZES */
		int sizes = stream.readInt32();
		for (unsigned int i = 0; i < result.numOfSegments && (i * 3) < sizes; ++i) {
			int size = (stream.readByte() | stream.readByte() << 8 |
					stream.readByte() << 16 | 0 << 24);
			result.segmentSizes.push_back(size);
		}

		/* ADDITINAL DATA */
		std::vector<std::string> adds = str::split(stream.readString(), '|');
		result.forWhom = adds.at(0);
		result.uploader = adds.at(1);
		result.comment = adds.at(2);

		result.valid = true; // TODO: add required checks before setting it!
        }
	return new YgoowHash(result);
}

