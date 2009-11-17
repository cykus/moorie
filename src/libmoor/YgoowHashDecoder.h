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

#ifndef YGOOWHASH_H__
#define YGOOWHASH_H__

#include <string>

#include "Tools.h"
#include "HashDecoder.h"
#include "HashInfo.h"

//! Ygoow hashfile data
class YgoowHashDecoder : public HashDecoder {
	public:
		~YgoowHashDecoder() {}

	public:
		virtual HashInfo decode(const std::string& hashcode);
};

#endif // YGOOWHASH_H__

