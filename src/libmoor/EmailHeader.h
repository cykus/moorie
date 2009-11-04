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
using namespace std;
using namespace boost;
struct EmailHeader
{
	string subject; //!< Email message subject
	string uniqueId; //!< Unique id of the message

	EmailHeader(string id, string subj): uniqueId(id) 
	    { subject = decode(subj); }

    private:
	    string decode( const string & );
	    string asciiDecode( const string& );
};

#endif

