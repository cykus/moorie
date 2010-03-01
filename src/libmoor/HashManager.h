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

#ifndef HASHMANAGER_H__
#define HASHMANAGER_H__

#include <list>
#include <string>

class Hash;
class HashDecoder;

class HashManager {
	public:
// 		static std::list<HashInfo> fromFile(const std::string& filename);
		/**
		 * Creates hash object for specified hashcode.
		 * @param hashcode Hashcode string.
		 * @return Hash object.
		 */
		static Hash* fromString(const std::string& hashcode);

	private:
		/**
		 * Gets decoder for specified hashcode.
		 * @param hashcode Hashcode string to decode.
		 * @return Hash decoder.
		 */
                //static HashDecoder* getHashDecoder(const std::string& hashcode);

	private:
		HashManager();
		HashManager(const HashManager&);
		HashManager& operator=(const HashManager&);
};

#endif // HASHMANAGER_H__
