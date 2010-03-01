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

#ifndef HASHINFO_H__
#define HASHINFO_H__

/** @file */

#include <string>
#include <vector>

/**
 * Stores information decoded from hash.
 */
struct HashInfo {
	/**
	 * Stores data regarding mailbox.
	 */
	struct MboxAccount {
		std::string name;     ///< Mailbox address (hostname).
		std::string login;    ///< Mailbox login.
		std::string password; ///< Mailbox password.
	};
	
	bool valid; //!< Flag indicating whether hash is valid, set on decoding.
	char verMaj;
	char verMin;
	std::string fileName; //!< Downloaded file name (as provided in hashfile).
	long fileSize;        ///< Size of the upload.
	unsigned int crc;     ///< Control checksum.
	int numOfSegments;    ///< Number of segments the upload was splitted into.

	/**
	 * Stores size of segments.
	 * If this variable is set to 0 (and hash IS #valid) then it most probably
	 * means that segments don't have the same size and one must try checking
	 * member #segmentSizes.
	 * @see segmentSizes
	 */
	int segmentSize;

	/**
	 * Stores sizes of each segment.
	 * This member is set only if segments are of different sizes.
	 * @see segmentSize
	 */
	std::vector<int> segmentSizes;

	std::vector<MboxAccount> accounts; ///< Mailboxes data.
	std::string forWhom;
	std::string accessPasswd; ///< Download password for this hash.
	std::string editPasswd;   ///< Password required to edit this hash.
	std::string coverURL;
	std::string descURL;
	std::string fullTitle;
	std::string uploader;
	std::string comment;
	std::string hashString; //!< raw hash std::string (base64 encoded)

	/** 
	 * Creates hash info object.
	 * Sets basic fields to default values, such as #valid to @c false.
	 */
	HashInfo() : valid(false) {}
};

/**
 * Compares two hashes.
 * @param lhs Left side hash.
 * @param rhs Right side hash.
 * @return If equal (true) or not (false).
 */
inline bool operator==(const HashInfo& lhs, const HashInfo& rhs) {
	return (lhs.fileName == rhs.fileName);
}

#endif // HASHINFO_H__
