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

#ifndef __STATUS_H
#define __STATUS_H

#include <string>

//! High-level status some are reported to the user
class Status
{
	public:
                //! Possible states
		enum State
		{
			Awaiting,         //!< No segment is currently being downloaded
			ProcessStarted,   //!< Download subprocess started
			Connecting,       //!< Connecting or logging to mailbox
			Connected,        //!< Succesfully connected to mailbox
                        Uploaded,         //!< All segments send
                        Uploading,
			Downloading,      //!< A segment is being downloaded
			ConnectionError,  //!< Network or mailbox login error
                        SegmentError,     //!< CRC or other error for segment
                        FileError,        //!< CRC or other error for file
			Downloaded,       //!< All segments downloaded
			GivingUp,         //!< No (more) valid mailboxes
			Finished		  //!< Download finished
		};

		Status::State state; //!< Current state
                int downloadSegment; //!< Number of downloaded segments
		unsigned int speed; //!< Download speed (bytes per second)
                unsigned int bytesRead; //!< number of bytes downloaded so far
                std::string mailboxName; //!< name of mailbox
                bool downloadPaused; //!< Pause/Play upload

		Status();
                Status(int downloadSegment, unsigned int speed, unsigned int  bytesRead, std::string mailboxName, Status::State state);
		Status(const Status &s);

		/*!
		 * Returns current state as a string
		 */
		std::string getStateText() const;
};

#endif

