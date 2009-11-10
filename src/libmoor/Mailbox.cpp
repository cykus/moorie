#include "Mailbox.h"

CMailBox::CMailBox(const std::string &usr, const std::string &passwd)
	: user(usr)
	, password(passwd)
	, bufferPos(0)
	, buffer(NULL)
	, usecnt(0)
	, failcnt(0)
	, successcnt(0)
	, prefweight(0)
	, scoreNeedsUpdate(true)
	, validAccount(true)
	, bytesRead( 0 )
{
	handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_AUTOREFERER, 1);
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(handle, CURLOPT_USERAGENT, "User-Agent: Mozilla/5.0 (compatible;)");
	curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "");
	curl_easy_setopt(handle, CURLOPT_HEADER, 0);
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, CMailBox::_writeData);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0); // don't verify peer certificates
	//curl_easy_setopt(handle, CURLOPT_FORBID_REUSE, 1);
	//curl_easy_setopt(handle, CURLOPT_FRESH_CONNECT, 1);
	//curl_easy_setopt(handle, CURLOPT_VERBOSE, 1);

	segDownload = false;
}

CMailBox::~CMailBox() {
	curl_easy_cleanup(handle);
	if (buffer)
		delete [] buffer;
}

void CMailBox::setCookie( std::string cookie ) const
{
	curl_easy_setopt(handle, CURLOPT_COOKIE, cookie.c_str());
}

string& CMailBox::doGet(std::string url, bool  header)
{
//	mutex::scoped_lock lock( speedMutex );
	bytesRead = 0;
	startTime = posix_time::microsec_clock::universal_time();
//	lock.unlock();

	stopFlag = false;
	this->url = url;
//	LOG(Log::Debug, "GET: " + url);
	bufferPos = 0;
	curl_easy_setopt(handle, CURLOPT_HEADER, header);
	curl_easy_setopt(handle, CURLOPT_HTTPGET, 1);
	curl_easy_setopt(handle, CURLOPT_URL, this->url.c_str());
	CURLcode status = curl_easy_perform(handle);
	if (status != 0)
	{
//		LOG(Log::Error, format("curl_easy_perform() error: %s") % curl_easy_strerror(status));
	}
//	LOG(Log::Debug,"Before request");
	requestComplete();
	return this->result;
}

string& CMailBox::doPost(std::string url, std::string vars, bool header)
{
//	mutex::scoped_lock lock( speedMutex );
	bytesRead = 0;
	startTime = posix_time::microsec_clock::universal_time();
//	lock.unlock();
	stopFlag = false;
	this->url = url;
	this->vars = vars;
//	LOG(Log::Debug, "POST: " + url + " DATA: " + vars);
	bufferPos = 0;
	curl_easy_setopt(handle, CURLOPT_HEADER, header);
	curl_easy_setopt(handle, CURLOPT_POST, 1);
	curl_easy_setopt(handle, CURLOPT_POSTFIELDS, this->vars.c_str());
	curl_easy_setopt(handle, CURLOPT_URL, this->url.c_str());
	CURLcode status = curl_easy_perform(handle);
	if (status != 0)
	{
//		LOG(Log::Error, format("curl_easy_perform() error: %s") % curl_easy_strerror(status));
		cout << curl_easy_strerror(status) << endl;
	}
	requestComplete();
	return this->result;
}

void CMailBox::requestComplete()
{
//	LOG_ENTER("Mailbox::requestComplete");
//	if (getState() == DownloadIP)
//		setState(DownloadDone);
	if(buffer)
		this->result=string(buffer);
	else
		this->result="";
//	parseResponse();
}

std::string CMailBox::getUser() const
{
	return user;
}

std::string CMailBox::getPassword() const
{
	return password;
}

std::string CMailBox::escape(std::string q)
{
	char *e = curl_easy_escape(handle, q.c_str(), 0);
	std::string s(e);
	return s;
}

size_t CMailBox::_writeData(void *buffer, size_t size, size_t nmem, void *ptr)
{
	CMailBox *myMailBox = static_cast<CMailBox *>(ptr);
	return myMailBox->writeData(buffer, size, nmem);
}

size_t CMailBox::writeData(void *buffer, size_t size, size_t nmem)
{
	const int n = size * nmem;
//	LOG(Log::Trace, format("read %d bytes") % n);

//	mutex::scoped_lock lock( speedMutex );
	bytesRead += n;
//	lock.unlock();

//	if (getState() == Mailbox::DownloadIP)
//	{
//		segment->writeData(static_cast<char *>(buffer), n);
//	}
	if (segDownload == true) {
		file -> write(static_cast<char *>(buffer), n);
		cout << "Bytes readed: " << bytesRead << " Writed: " << n << endl;
	}
	else
	{
		if (bufferPos + n >= CMailBox::BUFFER_SIZE)
		{
//			LOG(Log::Error, "too many bytes, Mailbox BUFFER_SIZE too small");
			return 0;
		}
		if (this->buffer == NULL)
			this->buffer = new char [CMailBox::BUFFER_SIZE];
		memcpy(this->buffer + bufferPos, buffer, n);
		bufferPos += n;
		this->buffer[bufferPos] = 0;
	}
	if ( stopFlag ) {
		//TODO
	}
	return n;
}

void CMailBox::addHeader(const EmailHeader &hdr)
{
	headers.push_back(hdr);
}

void CMailBox::addHeaderLink(std::string link)
{
	segments_links.push_back(link);	
}

void CMailBox::clearHeaders()
{
	headers.clear();
}

std::list<EmailHeader> CMailBox::getHeaders() const
{
	return headers;
}

std::vector<string> CMailBox::getLinks() const
{
	return segments_links;
}

string CMailBox::getLink(int seg) {
	int counter; counter = 0;
	ostringstream ss;
	ss << seg;
	string id = ss.str();
	boost::regex hreg("\\[" + id + "\\]"); // match "[crc][id]"
	boost::smatch match; 
	for (std::list<EmailHeader>::const_iterator it = headers.begin(); it!=headers.end(); it++)
	{
		counter++;
		if (boost::regex_search(it->subject, match, hreg))
		{
			cout << match[0] << endl;
			//
                        // get crc for this segment from email header
//			const std::string cksum = match[1];
//			return cksum;
//			LOG(Log::Debug, "Found header: " + it->subject);
//			fromHex(cksum, crc);
//			bytesWritten = 0;
//			crcRes.reset();
//			file = new std::ofstream(fileName.c_str(), std::ofstream::binary); //TODO: blad?
//			mailbox->downloadRequest(*it, this);
			break;
		}

	} 
	return segments_links.at(counter);
}

int CMailBox::downloadSeg(bool dec) {
	if (dec == true) {
		segDownload = true;
		file = new std::ofstream("seg_tmp", std::ofstream::binary | std::ofstream::app);
	}
	else 
		segDownload = false;
}
