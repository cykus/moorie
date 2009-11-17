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

#ifndef MOORHUNTHASH_H__
#define MOORHUNTHASH_H__

#include <string>

//#include <MailSystem/Account.h>
#include "HashDecoder.h"

class MoorhuntHashDecoder : public HashDecoder {
	public:
		~MoorhuntHashDecoder() {}
		
	public:
		virtual HashInfo decode(const std::string& hashcode);
};

#endif // MOORHUNTHASH_H__

