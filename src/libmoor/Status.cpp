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
		case Status::ConnectionError:
                        return "Błąd połączenia";
		case Status::SegmentError:
                        return "Nieprawidłowy CRC segmentu";
                case Status::FileError:
                        return "Nieprawidłowy CRC pliku";
		case Status::GivingUp:
                        return "Poddaję się i płaczę";
		case Status::Finished:
                        return std::string("Pobieranie Zakończone");
		default:
			return "";
	}
}

