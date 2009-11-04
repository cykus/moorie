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
using namespace boost;

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
	
	string result; //Variable that contains result of request

//	mutable mutex speedMutex;
	posix_time::ptime startTime; //!< Connection start time, used to measure speed
	unsigned int bytesRead; //!< Number of bytes processed, used to measure speed

	static const int BUFFER_SIZE = 1024*256; //!< Size of temporary buffer (256kB)

	protected:
		static size_t _writeData(void *buffer, size_t size, size_t nmem, void *ptr);
		virtual size_t writeData(void *buffer, size_t size, size_t nmem);
		
		void setCookie( string ) const;
		string& doGet(string url, bool header=false);
		string& doPost(string url, string vars, bool header=false);
		string getUser() const;
		string getPassword() const;
		string escape(string q);
		
		void requestComplete();
	
	public:
		CMailBox(const std::string &usr, const std::string &passwd);
		virtual int Login() = 0;
		virtual void getHeadersRequest() = 0;
		virtual ~CMailBox();
};


#endif
