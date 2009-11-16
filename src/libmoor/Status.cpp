#include "Status.h"

Status::Status()
{
}

Status::Status(int downloadSegment, unsigned int speed, unsigned int bytesRead, std::string mailboxName)
        : downloadSegment( downloadSegment )
        , speed( speed )
        , bytesRead( bytesRead )
        , mailboxName( mailboxName )
{
}

Status::Status( const Status &s )
{
        downloadSegment = s.downloadSegment;
	speed = s.speed;
        bytesRead = s.bytesRead;
        mailboxName = s.mailboxName;
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
		case Status::Merging:
			return "Merging";
		case Status::Merged:
//			return std::string("Merged, CRC ") + (crcOk ? "OK" : "FAIL");
			return std::string("Download complete!");
		case Status::ConnectionError:
			return "Connection Error";
		case Status::SegmentError:
			return "Segment CRC Error";
		case Status::GivingUp:
			return "Giving up";
		case Status::Finished:
                        return std::string("Download complete! ");
		default:
			return "";
	}
}

