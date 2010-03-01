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
#include "Log.h"

Log* Log::instance = NULL;

Log::Log(): minLvl(Log::None)
{
}

Log::~Log()
{
	boost::mutex::scoped_lock lock( handlesMutex );
	for (std::list<LogHandle *>::const_iterator it = handles.begin(); it != handles.end(); it++)
		delete *it;
}

std::string Log::levelString(Log::Level lvl)
{
	switch (lvl)
	{
		case Log::Trace:  return std::string("Ślad: ");
		case Log::Debug:  return std::string("");
		case Log::Info:   return std::string("Info: ");
		case Log::Leave:  return std::string("Opuszczenie: ");
		case Log::Enter:  return std::string("Wejście: ");
		case Log::Fine:   return std::string("[FINE] ");
		case Log::Warn:   return std::string("Ostrzeżenie: ");
		case Log::Error:  return std::string("Błąd: ");
		default:
				  return "";
	};
}

void Log::addHandle(LogHandle *h)
{
	boost::mutex::scoped_lock lock( handlesMutex );
	handles.push_back(h);
	if (h->getLevel() < minLvl)
		minLvl = h->getLevel();

}

void Log::log(Log::Level lvl, const char *msg)
{
	if (lvl >= minLvl)
	{
		std::string fmsg = levelString(lvl) + msg;

		boost::mutex::scoped_lock lock( handlesMutex );
		for (std::list<LogHandle *>::const_iterator it = handles.begin(); it != handles.end(); it++)
		{
			LogHandle *h = *it;
			if (lvl >= h->getLevel())
				h->log(fmsg.c_str());
		}
	}
}

void Log::log(Log::Level lvl, const boost::format &msg)
{
	if (lvl >= minLvl)
	{
		log(lvl, msg.str().c_str());
	}
}

void Log::log(Log::Level lvl, const char *msg, int len)
{
	if (lvl >= minLvl)
	{
		std::string str(msg, len);
		log(lvl, str.c_str());
	}
}

void Log::log(Log::Level lvl, const std::string &msg)
{
	log(lvl, msg.c_str());
}

LogHandle::~LogHandle()
{
}

LogFileHandle::LogFileHandle(const std::string &filename, Log::Level lvl): LogHandle(lvl)
{
	stream = new std::ofstream(filename.c_str());
}

LogFileHandle::~LogFileHandle()
{
	stream->close();
	delete stream;
}
void LogFileHandle::log(const char *msg)
{
	*stream << msg << std::endl;
}
LogConsoleHandle::LogConsoleHandle(Log::Level lvl): LogHandle(lvl)
{
}

LogConsoleHandle::~LogConsoleHandle()
{
}
void LogConsoleHandle::log(const char *msg)
{
	std::cout << msg << std::endl;
}

