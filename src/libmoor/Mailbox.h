#ifndef _MAILBOX_H_
#define _MAILBOX_H_

#include <string>
#include <list>
#include <stdio.h>
#include <curl/curl.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

class CMailBox {

	CURL *handle; //!< Connection handle using curl easy interface
	string url; //!< Current URL
	string vars; //<! Variables for current POST request (used for CURLOPT_POSTFIELDS)
	string user; //!< User login
	string password; //!< User password
	int bufferPos; //!< Number of bytes written by subsequent writeData calls
	char *buffer; //!< Temporary buffer for incoming data, size defined by BUFFER_SIZE
//	list<EmailHeader> headers; //!< Email headers read by getHeadersRequest() call
//	State state; //!< Current state of this Mailbox object
//	Segment *segment; //!< Currently downloaded Segment
	int usecnt; //!< Number of times this mailbox was used
	int successcnt; //<! Number of successful uses of this mailbox
	int failcnt; //<! Number of failuers for this mailbox
	int prefweight; //<! Preference weight for this mailbox
	int score; //<! Current score of this mailbox, based on usecnt, successful, failcnt, prefweight
	bool scoreNeedsUpdate; //<! Flag indicating that score needs to be recalculated
	bool validAccount; //<! Flag indicating whether this is valid mailbox
	int uSeq; //!< Unique id of this Mailbox object derived from seq
	bool stopFlag; //!< stops transfer

	public:
		CMailBox();
		virtual ~CMailBox();
};


#endif
