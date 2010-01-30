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
			return "Awaiting";
		case Status::Connecting:
			return "Connecting";
		case Status::Connected:
			return "Connected";
		case Status::Downloading:
			return "Downloading";
		case Status::Downloaded:
			return "All segments downloaded";
		case Status::ConnectionError:
			return "Connection Error";
		case Status::SegmentError:
			return "Segment CRC Error";
                case Status::FileError:
                        return "File CRC Error";
		case Status::GivingUp:
			return "Giving up";
		case Status::Finished:
                        return std::string("Download complete! ");
		default:
			return "";
	}
}

