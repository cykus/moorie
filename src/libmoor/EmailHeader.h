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

std::string EncodeHeader(std::string filename, std::string segCRC, std::string fileCRC, int seg);

#endif

