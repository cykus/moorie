#ifndef HASHINFO_H__
#define HASHINFO_H__

#include <vector>
#include <list>
#include <string>

struct HashInfo {
	bool valid; //!< flag indicating whether hash is valid, set on decoding
	char verMaj;
	char verMin;
	std::string fileName; //!< downloaded file name (as provided in hashfile)
	long fileSize;
	unsigned int crc;
	int numOfSegments;
	int segmentSize;
	std::vector<std::string> accounts;
	std::vector<int> ids;
	std::string forWhom;
	std::string accessPasswd;
	std::string editPasswd;
	std::string coverURL;
	std::string descURL;
	std::string fullTitle;
	std::string uploader;
	std::string comment;
	std::string hashString; //!< raw hash std::string (base64 encoded)
	std::string hash; ///< suppplied hash
};

inline bool operator==(const HashInfo& lhs, const HashInfo& rhs) {
	return (lhs.fileName == rhs.fileName);
}

#endif // HASHINFO_H__
