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

#ifndef __LOG_H
#define __LOG_H

#include <string>
#include <list>
#include <boost/format.hpp>
#include <boost/thread/mutex.hpp>
#include <fstream>
#include <iostream>

#define LOG(level, message) Log::getLog()->log(level, message)
#define LOG_BUFFER(level, message, len) Log::getLog()->log(level, message, len)
#define LOG_ENTER(message)  Log::getLog()->log(Log::Enter, message)
#define LOG_LEAVE(message)  Log::getLog()->log(Log::Leave, message)

class LogHandle;

//! Log handling singleton class
class Log
{
	public:
		//! Log levels
		enum Level
		{
			Trace,
			Debug,
			Info,
			Leave,
			Enter,
			Fine,
			Warn,
			Error,
			None  //!< Used internally - don't use
		};
	private:
		static Log *instance; //!< Log object instance (singleton desing pattern)
		Log::Level minLvl; //!< Minimum log level
		boost::mutex handlesMutex;
		std::list<LogHandle *> handles; //!< Logging handles the messages will be forwarded to

		//! Converts Level enum to string
		static std::string levelString(Log::Level lvl);

		Log();
		~Log();

	public:
		//! Returns Log object instance creating one if necessary
		static Log* getLog()
		{ 
			if (instance == NULL)
				instance = new Log();
			return instance;
		}

		/*!
		 * Adds new LogHandle to the list of handles that will receive log messages
		 *
		 * @param h handle
		 */
		void addHandle(LogHandle *h);

		void log(Log::Level lvl, const char *msg);
		void log(Log::Level lvl, const char *msg, int len);
		void log(Log::Level lvl, const std::string &msg);
		void log(Log::Level lvl, const boost::format &msg);
};

//! Abstract log handle
class LogHandle
{
	private:
		Log::Level level;
	public:
		LogHandle(Log::Level lvl): level(lvl) {}
		virtual ~LogHandle();
		virtual void log(const char *msg) = 0;
		Log::Level getLevel() const { return level; }
};

//! Log handle that outputs log messages to a file
class LogFileHandle: public LogHandle
{
	private:
		std::ofstream *stream;
	public:
		LogFileHandle(const std::string &filename, Log::Level lvl);
		virtual ~LogFileHandle();
		virtual void log(const char *msg);
};
//! Log handle that outputs log messages to a console
class LogConsoleHandle: public LogHandle
{
	private:
		std::ofstream *stream;
	public:
		LogConsoleHandle(Log::Level lvl);
		virtual ~LogConsoleHandle();
		virtual void log(const char *msg);
};
#endif

