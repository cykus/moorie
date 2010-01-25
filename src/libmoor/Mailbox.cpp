#include "Mailbox.h"

#include "StringUtils.h"

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
        , allBytesRead( 0 )
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
//  	curl_easy_setopt(handle, CURLOPT_VERBOSE, 1);

	segDownload = false;
}

CMailBox::~CMailBox() {
	curl_easy_cleanup(handle);
	if (buffer)
		delete [] buffer;
}

void CMailBox::setFileName(std::string file) {
	filename = file;
}

void CMailBox::setFileCRC(int crc) {
	std::stringstream ss;
	ss << std::setw(8) << std::setfill('0') << std::hex << crc;
	std::cout << std::hex << crc;
	fileCRC = ss.str();
	boost::to_upper(fileCRC);
	LOG( Log::Info, boost::format("File CRC: %1%") %fileCRC);

}
void CMailBox::setCookie( std::string cookie ) const
{
	curl_easy_setopt(handle, CURLOPT_COOKIE, cookie.c_str());
}

std::string& CMailBox::doGet(std::string url, bool  header)
{
//	mutex::scoped_lock lock( speedMutex );
	bytesRead = 0;
	startTime = boost::posix_time::microsec_clock::universal_time();
//	lock.unlock();
	stopFlag = false;
	this->url = url;
//	LOG(Log::Debug, "GET: " + url);
	bufferPos = 0;
	curl_easy_setopt(handle, CURLOPT_HEADER, header);
	curl_easy_setopt(handle, CURLOPT_HTTPGET, 1);
	curl_easy_setopt(handle, CURLOPT_URL, this->url.c_str());
 	status = curl_easy_perform(handle);
// 	if (status != 0)
// 	{
// 		LOG(Log::Error, boost::format("curl_easy_perform() error: %s") % curl_easy_strerror(status));
// 	}
//	LOG(Log::Debug,"Before request");
	requestComplete();
	return this->result;
}

std::string& CMailBox::doPost(std::string url, std::string vars, bool header)
{
//	mutex::scoped_lock lock( speedMutex );
	bytesRead = 0;
	startTime = boost::posix_time::microsec_clock::universal_time();
//	lock.unlock();
	stopFlag = false;
	this->url = url;
	this->vars = vars;
	LOG(Log::Debug, "POST: " + url + " DATA: " + vars);
	bufferPos = 0;
	curl_easy_setopt(handle, CURLOPT_HEADER, header);
	curl_easy_setopt(handle, CURLOPT_POST, 1);
	curl_easy_setopt(handle, CURLOPT_POSTFIELDS, this->vars.c_str());
	curl_easy_setopt(handle, CURLOPT_URL, this->url.c_str());
	status = curl_easy_perform(handle);
	if (status != 0)
	{
//		cout << curl_easy_strerror(status) << endl;
		LOG(Log::Error, boost::format("curl_easy_perform() error: %s") % curl_easy_strerror(status));
	}
	requestComplete();
	return this->result;
}

static size_t read_callback(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t retcode;

  /* in real-world cases, this would probably get this data differently
	as this fread() stuff is exactly what the library already would do
	by default internally */
	retcode = fread(ptr, size, nmemb, stream);

	std::cout << "retcode: " << retcode << std::endl;

	return retcode;
}

std::string& CMailBox::doSMTPUpload(std::string server, std::string login, std::string password, std::string filename) {
}

std::string& CMailBox::doHTTPUpload(std::string url, variables myvars, std::string filename, bool header) {
	struct curl_httppost* post = NULL;
	struct curl_httppost* last = NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect:";

	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
  	curl_easy_setopt(handle, CURLOPT_HEADER, header);

	curl_formadd(&post, &last, CURLFORM_COPYNAME, myvars.to_form.c_str(), CURLFORM_COPYCONTENTS, myvars.to_address.c_str(), CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, myvars.subject_form.c_str(), CURLFORM_COPYCONTENTS, myvars.subject.c_str(), CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, myvars.body_form.c_str(), CURLFORM_COPYCONTENTS, myvars.body.c_str(), CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, myvars.file_form.c_str(), CURLFORM_FILE, filename.c_str(), CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, myvars.file_form.c_str(), CURLFORM_COPYCONTENTS, filename.c_str(), CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, myvars.submit_form.c_str(), CURLFORM_COPYCONTENTS, myvars.submit.c_str(), CURLFORM_END);

	headerlist = curl_slist_append(headerlist, buf);


	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(handle, CURLOPT_HTTPPOST, post);

	status = curl_easy_perform(handle);
	if (status != 0)
	{
		LOG(Log::Error, boost::format("curl_easy_perform() error: %s") % curl_easy_strerror(status));
	}

// 	curl_formfree(post);
// 	curl_slist_free_all (headerlist);

	requestComplete();

	return this->result;
}

void CMailBox::requestComplete()
{
//	LOG_ENTER("Mailbox::requestComplete");
//	if (getState() == DownloadIP)
//		setState(DownloadDone);
	if(buffer)
		this->result = std::string(buffer);
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

std::string CMailBox::unescape(std::string q)
{
	char *e = curl_easy_unescape(handle, q.c_str(), 0, NULL);
	std::string s(e);
	return s;
}

std::string CMailBox::getFileCRC() {
	return fileCRC;
}

std::string CMailBox::getSegCRC(std::string filename) {
	crcRes.reset();
	std::ifstream in(filename.c_str(), std::ifstream::binary);
	while (!in.eof()) {
		in.read(buffer, READ_BUFFER_SIZE);
		int n = in.gcount();
		crcRes.process_bytes(buffer, n);
	}
	in.close();

	std::stringstream ss;
	ss << std::setw(8) << std::setfill('0')  << std::hex << crcRes.checksum();
	segCRC = ss.str();

	boost::to_upper(segCRC);

	return segCRC;
}

void CMailBox::calculateFileCRC(std::string filename) {
	fileCRC = getSegCRC(filename); // nie ma to jak skroty :)
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
		tmp_file -> write(static_cast<char *>(buffer), n);
//		cout << "Bytes readed: " << bytesRead << " Writed: " << n << endl;
                allBytesRead += n;
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

std::vector<std::string> CMailBox::getLinks() const
{
	return segments_links;
}

std::string CMailBox::getLink(int seg) {
	int counter = 0;
	std::ostringstream ss;
	ss << seg;
	std::string id = ss.str();
	segNumber = id;
	boost::regex hreg("\\[" + fileCRC + "\\].+?\\[" + id + "\\]"); // match "[crc][id]"
	for (std::list<EmailHeader>::const_iterator it = headers.begin(); it!=headers.end(); it++)
	{
		if (boost::regex_search(it->subject, hreg))
			break;
		else
			++counter;
	}
	//LOG( Log::Debug, boost::format( "%1% %2%" ) %counter %segments_links.at(counter));
	return segments_links.at(counter);
}

int CMailBox::downloadSeg() {
	segDownload = true;
	segOK = false;
	crcRes.reset();
	std::string tmpfile = filename+".seg";
	boost::filesystem::remove(tmpfile.c_str());
	tmp_file = new std::ofstream(tmpfile.c_str(), std::ofstream::binary | std::ofstream::app);
	return 0;
}

int CMailBox::downloadSegDone() {
	crcRes.reset();
	segDownload = false;
	tmp_file->close();
	std::string tmpfile = filename+".seg";
	std::ifstream in(tmpfile.c_str(), std::ifstream::binary);
	while (!in.eof()) {
		in.read(buffer, READ_BUFFER_SIZE);
		int n = in.gcount();
		crcRes.process_bytes(buffer, n);
	}
	in.close();
//	cout << crcRes.checksum() << endl;
	std::stringstream ss;
	ss << std::setw(8) << std::setfill('0')  << std::hex << crcRes.checksum();
	segCRC = ss.str();
//	cout << segCRC << endl;
	LOG( Log::Info, segCRC);
// 	int counter; counter = 0;
// 	segNumber = seg;
// 	ostringstream ss;
//	ss << seg;
//	string id = ss.str();
	boost::to_upper(segCRC);
	boost::regex hreg("\\[" + fileCRC + "\\]\\[" + segCRC + "\\]\\[" + segNumber + "\\]"); // match "[crc][id]"
	boost::smatch match;
	for (std::list<EmailHeader>::const_iterator it = headers.begin(); it!=headers.end(); it++)
	{
		if (boost::regex_search(it->subject, match, hreg))
		{
//			cout << match[0] << endl;
			segOK = true;
			LOG( Log::Info, "Seg CRC OK");
			break;
		}
	}

	if (segOK == true) {
		std::ifstream segfile(tmpfile.c_str(), std::ifstream::binary);
		std::ofstream myfile(filename.c_str(), std::ofstream::binary | std::ofstream::app);
		while (!segfile.eof()) {
			segfile.read(buffer, READ_BUFFER_SIZE);
			int n = segfile.gcount();
			crcRes.process_bytes(buffer, n);
			myfile.write(buffer, n);
		}
		boost::filesystem::remove(tmpfile.c_str());
		return 0;
	} else {
		boost::filesystem::remove(tmpfile.c_str());
		return 1;
	}
}

int CMailBox::checkHeaders(int numOfSegments) {
	int segments = 0;
	int lost = 0;
	for (int i = 1; i <= numOfSegments; i++) {
		std::ostringstream ss;
		ss << i;
		std::string id = ss.str();
		boost::regex hreg("\\[" + fileCRC + "\\].+?\\[" + id + "\\]"); // match "[crc][id]"
		boost::smatch match;
		for (std::list<EmailHeader>::const_iterator it = headers.begin(); it!=headers.end(); it++)
		{
			if (boost::regex_search(it->subject, match, hreg))
			{
				segments++;
				break;
			}
		}

	}
	lost = numOfSegments - segments;
	LOG( Log::Info, boost::format("Brakujących segmentow: %1%") %lost);
	return segments;
}

unsigned int
CMailBox::countAvailableSegments(unsigned int segment) {
	unsigned int segments = 0;
	boost::regex re("\\[" + fileCRC + "\\].+?\\[(\\d+)\\]");
	boost::smatch match;
	std::list<EmailHeader>::const_iterator it = headers.begin();
	for (; it != headers.end(); ++it) {
		if (boost::regex_search(it->subject, match, re)) {
			if ((match.size() == 2) && match[1].matched &&
					(str::toInt(match[1].str()) > segment)) // > czy >= (co z segmentem 0? - czy segmenty sa oznaczane od 0 czy 1 ?
					++segments;
		}
	}

	return segments;
}

unsigned int CMailBox::getBytesRead() {
        return allBytesRead;
}
unsigned int CMailBox::getSpeed() const
{
        const boost::posix_time::ptime currTime = boost::posix_time::microsec_clock::universal_time();
        const boost::posix_time::time_duration d = currTime - startTime;
        if ( bytesRead > 0 && d.total_seconds() > 0 ) {
                const double b = ( static_cast<double>( bytesRead ) / d.total_seconds() );
                return static_cast<int>( b );
        }
        return 0;
}
