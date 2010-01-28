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
			Downloading,      //!< A segment is being downloaded
			ConnectionError,  //!< Network or mailbox login error
                        SegmentError,     //!< CRC or other error for segment
                        FileError,        //!< file or other error for segment
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

