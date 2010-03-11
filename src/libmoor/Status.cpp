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

#include "Status.h"

Status::Status()
{
}

Status::Status(int downloadSegment, unsigned int speed, unsigned int bytesRead, std::string mailboxName, Status::State state)
        : downloadSegment( downloadSegment )
        , speed( speed )
        , bytesRead( bytesRead )
        , mailboxName( mailboxName )
        , state( state )
{
}

Status::Status( const Status &s )
{
        downloadSegment = s.downloadSegment;
	speed = s.speed;
        bytesRead = s.bytesRead;
        mailboxName = s.mailboxName;
        downloadPaused = s.downloadPaused;
}

std::string Status::getStateText() const
{
	switch (state)
	{
		case Status::Awaiting:
                        return "Oczekiwanie";
		case Status::Connecting:
                        return "Łączenie";
		case Status::Connected:
                        return "Połączono";
		case Status::Downloading:
                        return "Pobieranie";
		case Status::Downloaded:
                        return "Pobrano wszystkie segmenty";
                case Status::Uploaded:
                        return "Wysłano wszystkie segmenty";
                case Status::Uploading:
                        return "Wysyłanie";
		case Status::ConnectionError:
                        return "Błąd połączenia";
		case Status::SegmentError:
                        return "Nieprawidłowy CRC segmentu";
                case Status::FileError:
                        return "Nieprawidłowy CRC pliku";
		case Status::GivingUp:
                        return "Poddaję się i płaczę";
		case Status::Finished:
                        return "Pobieranie Zakończone";
                case Status::FinishedUpload:
                        return "Wysyłanie Zakończone";
		default:
			return "";
	}
}

