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

#ifndef HASH_H__
#define HASH_H__

/** @file */

#include <string>
#include "HashInfo.h"

class Hash {
	public:
		explicit Hash(HashInfo hashInfo);
		virtual ~Hash() =0;

	public:
		/**
		 * Verifies hash download password against specified.
		 * @param password String to verify hash password against.
		 * @return Whether passwords match.
		 */
		virtual bool checkAccessPassword(const std::string& password);

		/**
		 * Gets object which stores hash data.
		 * @return Object storing hash data.
		 * @see HashInfo
		 */
		const HashInfo& getInfo() const { return model_; }

	private:
		HashInfo model_; ///< Stores hash data.
};

/**
 * Compares two hashes.
 * Current implementation forwards comparison to op== working on #HashInfo
 * objects.
 * @param lhs Left side hash.
 * @param rhs Right side hash.
 * @return Comparison result.
 * @see HashInfo
 * @see operator==(const HashInfo&, const HashInfo&)
 */
inline bool operator==(const Hash& lhs, const Hash& rhs) {
	return (lhs.getInfo() == rhs.getInfo());
}

#endif // HASH_H__
