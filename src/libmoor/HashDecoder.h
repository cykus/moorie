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

#ifndef HASHDECODER_H__
#define HASHDECODER_H__

#include <string>
#include <boost/shared_ptr.hpp>
#include "Hash.h"

class HashDecoder {
	public:
		virtual ~HashDecoder() {}

	public:
		/**
		 * Decodes supplied hashcode string.
		 * @param hashcode Hashcode string to decode.
		 * @return Decoded hash object.
		 */
		virtual Hash* decode(const std::string& hashcode) =0;
};

#endif // HASHDECODER_H__
