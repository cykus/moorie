#ifndef _MAILBOX_H_
#define _MAILBOX_H_

#include <string>
#include <list>
#include <cstdio>
#include <cstdlib>
#include <curl/curl.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/regex.hpp>
#include <boost/crc.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "EmailHeader.h"
#include "Log.h"
#include "Decoder.h"

class CMailBox {

	CURL *handle; //!< Connection handle using curl easy interface
	std::string url; //!< Current URL
	std::string vars; //!< Variables for current POST request (used for CURLOPT_POSTFIELDS)
        std::string mailbox; //!< Mailbox Name
	std::string user; //!< User login
	std::string password; //!< User password
	std::string filename;
	int bufferPos; //!< Number of bytes written by subsequent writeData calls
	char *buffer; //!< Temporary buffer for incoming data, size defined by BUFFER_SIZE
	std::list<EmailHeader> headers; //!< Email headers read by getHeadersRequest() call
	std::vector<std::string> segments_links;
//      State state; //!< Current state of this Mailbox object
//	Segment *segment; //!< Currently downloaded Segment
	int usecnt; //!< Number of times this mailbox was used
        int successcnt; //!< Number of successful uses of this mailbox
        int failcnt; //!< Number of failuers for this mailbox
        int prefweight; //!< Preference weight for this mailbox
        int score; //!< Current score of this mailbox, based on usecnt, successful, failcnt, prefweight
        bool scoreNeedsUpdate; //!< Flag indicating that score needs to be recalculated
        bool validAccount; //!< Flag indicating whether this is valid mailbox
	int uSeq; //!< Unique id of this Mailbox object derived from seq
	bool stopFlag; //!< stops transfer

	std::string result; //Variable that contains result of request

//	mutable mutex speedMutex;
	boost::posix_time::ptime startTime; //!< Connection start time, used to measure speed
        unsigned int bytesRead; //!< Number of bytes processed, used to measure speed
		unsigned int bytesSend;
        unsigned int allBytesRead;

	static const int BUFFER_SIZE = 1024*256; //!< Size of temporary buffer (256kB)
	static const int READ_BUFFER_SIZE = 8192;

	// zmienne dla segmentow
	std::ofstream *tmp_file;
	bool segDownload;
	bool segOK;
	boost::crc_32_type crcRes;
	std::string segCRC;
	std::string segNumber;
	std::string fileCRC;
	CURLcode status;

	protected:
		static size_t _writeData(void *buffer, size_t size, size_t nmem, void *ptr);
		virtual size_t writeData(void *buffer, size_t size, size_t nmem);

		void setCookie(std::string) const;
		std::string& doGet(std::string url, bool header=false);
		std::string& doPost(std::string url, std::string vars, bool header=false);

		struct variables {
			std::string to_form; // adresat - formularz
			std::string to_address; // adresat
			std::string subject_form; // temat - formularz
			std::string subject; // tresc tematu
			std::string body_form; // tresc wiadomosc - formularz
			std::string body; // tresc wiadomosci
			std::string file_form; // zalacznik - formularz
			std::string file; // zalacznik
			std::string submit_form; // wyslij - formularz
			std::string submit; // wyslij
		};
		std::string& doSMTPUpload(std::string server, std::string login, std::string password, std::string filename);
		std::string& doHTTPUpload(std::string url, variables myvars, std::string filename, bool header);
                std::string getMailbox() const;
                std::string getUser() const;
		std::string getPassword() const;
		std::string escape(std::string q);
		std::string unescape(std::string q);

		std::string getSegCRC(std::string filename);

		void requestComplete();

		void addHeader(const EmailHeader &hdr);
		void addHeaderLink(std::string link);
		void clearHeaders();

		std::string getLink(int seg);
		int downloadSeg();
		int downloadSegDone();


	public:
                CMailBox(const std::string &name, const std::string &usr, const std::string &passwd);
		void setFileName(std::string file);
		void setFileCRC(int crc);
		virtual int loginRequest() = 0;
		/**
		 * Gets upload headers from specific mailbox.
		 */
		virtual void getHeadersRequest() = 0;
		virtual int downloadRequest(int seg) = 0;
		virtual int uploadRequest(std::string filename, std::string to, int seg) = 0;
		std::list<EmailHeader> getHeaders() const;
		std::vector<std::string> getLinks() const;
		/**
		 * @deprecated Use CMailBox::countAvailableSegments() instead.
		 * @see countAvailableSegments()
		 */
		int checkHeaders(int numOfSegments);
		
		/**
		 * Gets number of available segments for current mailbox.
		 * @param segment Segment number to start counting from.
		 * @return Number of available segments.
		 * @remarks getHeadersRequest() must be run beforehand.
		 * @see getHeadersRequest()
		 */
		unsigned int countAvailableSegments(unsigned int segment);
		unsigned int getBytesRead();
		unsigned int getSpeed() const;
		std::string getFileCRC();
		void calculateFileCRC(std::string filename);
		virtual ~CMailBox();

  private:
    CMailBox(const CMailBox&);
    CMailBox& operator=(const CMailBox&);
};


#endif
