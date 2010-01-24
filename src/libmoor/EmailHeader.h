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

#ifndef __EMAILHEADER_H
#define __EMAILHEADER_H

#include <string>

//! Email header data
struct EmailHeader
{
	std::string subject; //!< Email message subject
	std::string uniqueId; //!< Unique id of the message

	EmailHeader(const std::string& id, const std::string& subj)
    : uniqueId(id)
	{
    subject = decode(subj);
  }

    private:
	    std::string decode(const std::string&);
	    std::string asciiDecode(const std::string&);
};

std::string EncodeHeader(std::string filename, std::string segCRC, std::string fileCRC);

#endif

